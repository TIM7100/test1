using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using Microsoft.Win32;

namespace BurnSDCard
{
    public partial class Form1 : Form
    {
        // ===== 通信命令汇总 =====
        // 主机命令格式: [HEADER(0xAA)] [CMD] [Payload...] [XOR] [FOOTER(0x55)]
        // ACK 应答格式: [HEADER(0x55)] [SEQ_L] [SEQ_H] [STATUS] [FOOTER(0x55)] [XOR]
        // 注意: 主机命令和 ACK 的 [XOR][FOOTER] 顺序相反
        // --------------------------------------------------------------------------
        // 0x00  PROTO_CMD_START      开始烧录(加密) [AA 00 LEN(4) XOR 55]    等待ACK
        //      └─ ACK: [55 00 00 STATUS 55 XOR]  STATUS=0x00 表示就绪
        // 0x04  PROTO_CMD_START_PLAIN 开始烧录(明文) [AA 04 LEN(4) XOR 55]    等待ACK
        //      └─ ACK: [55 00 00 STATUS 55 XOR]  STATUS=0x00 表示就绪
        // 0x01  PROTO_CMD_DATA      数据包     [AA 01 SEQ(2) LEN(2) DATA XOR 55]
        //      └─ ACK: [55 SEQ(2) STATUS 55 XOR] STATUS=0x00 表示该包接收成功
        // 0x02  PROTO_CMD_LINK_TEST 链接测试   [AA 02 XOR 55]               等待ACK
        //      └─ ACK: [55 00 00 STATUS 55 XOR]  STATUS=0x00 表示链接正常
        // 0x03  PROTO_CMD_LINK_END  结束测试   [AA 03 XOR 55]               等待ACK
        //      └─ ACK: [55 00 00 STATUS 55 XOR]  STATUS=0x00 表示已结束测试
        // 0xFF  PROTO_CMD_END       烧录结束   [AA FF XOR 55]               无ACK
        // --------------------------------------------------------------------------
        // STATUS: 0x00=OK  0x01=CHECKSUM_ERR  0x02=OTHER_ERR
        // SEQ:    2字节(小端),START/LINK_TEST/LINK_END 固定为 0;DATA 为包序号
        //         SEQ 范围 0~65535,理论BIN上限 = 65535 × 65535 ≈ 4GB
        // LEN:    2字节(大端),单包最大 65535 字节
        // --------------------------------------------------------------------------
        // XOR 计算示例:
        //   主机命令 XOR = HEADER ^ CMD ^ ... (不含 FOOTER)
        //     START(加密): 0xAA ^ 0x00 ^ LEN(4)  帧: AA 00 LEN(4) XOR 55
        //     START(明文): 0xAA ^ 0x04 ^ LEN(4)  帧: AA 04 LEN(4) XOR 55
        //     LINK_TEST: 0xAA ^ 0x02 = 0xA8    帧: AA 02 A8 55
        //     LINK_END:  0xAA ^ 0x03 = 0xA9    帧: AA 03 A9 55
        //     END:       0xAA ^ 0xFF = 0x55    帧: AA FF 55 55
        //     DATA:      0xAA ^ 0x01 ^ SEQ_L ^ SEQ_H ^ LEN_H ^ LEN_L ^ DATA...
        //   ACK XOR = HEADER ^ SEQ_L ^ SEQ_H ^ STATUS ^ FOOTER (含 FOOTER)
        //     ACK OK:    0x55 ^ 0x00 ^ 0x00 ^ 0x00 ^ 0x55 = 0x00    帧: 55 00 00 00 55 00
        // --------------------------------------------------------------------------
        private const byte PROTO_HEADER = 0xAA;
        private const byte PROTO_CMD_START = 0x00;
        private const byte PROTO_CMD_START_PLAIN = 0x04;  // 明文模式(不加密)
        private const byte PROTO_CMD_DATA = 0x01;
        private const byte PROTO_CMD_LINK_TEST = 0x02;
        private const byte PROTO_CMD_LINK_END = 0x03;
        private const byte PROTO_CMD_END = 0xFF;
        private const byte PROTO_FOOTER = 0x55;

        private const byte ACK_HEADER = 0x55;
        private const byte ACK_STATUS_OK = 0x00;
        private const byte ACK_STATUS_CHECKSUM_ERR = 0x01;
        private const byte ACK_STATUS_OTHER_ERR = 0x02;

        private const int ACK_TIMEOUT_MS = 3000;        // 控制命令超时(START/LINK等)
        private const int DATA_ACK_TIMEOUT_MS = 10000;  // 数据包超时:SD写扇区偶发秒级抖动
        private const int MAX_RETRY = 3;

        // ===== AES-128-CBC 加密(密文写入SD,防抄录) =====
        // 注意: Key/IV 必须与下位机解密端写死的一致,各16字节
        private static readonly byte[] AES_KEY = Encoding.ASCII.GetBytes("BURN2026KEY@0123");
        private static readonly byte[] AES_IV  = Encoding.ASCII.GetBytes("0123456789ABCDEF");

        private SerialPort _serialPort;
        private byte[] _fileData;
        private bool _isBurning = false;
        private bool _stopRequested = false;
        private Thread _burnThread;
        private int _bytesSent = 0;

        private AutoResetEvent _ackEvent;
        private int _lastAckSeq;
        private int _lastAckStatus;
        private int _expectedSeq = -1;
        private bool _serialDisconnected = false;   // 串口被动断开处理标志,防止重复弹窗
        private System.Windows.Forms.Timer _portWatchTimer = null;   // 轮询检测串口是否被拔出
        private int _idleProbeCounter = 0;          // 空闲自检计数(每约1秒一次)
        private bool _probingPort = false;          // 正在做"关闭->重查->重开"探测,防重入
        private readonly object _rxLock = new object();          // 接收缓冲锁
        private readonly List<byte> _rxBuffer = new List<byte>(); // 接收累积缓冲,防止串口帧分片

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            RefreshPorts();
            // 下拉框默认选中项(否则DropDownList模式下SelectedItem为null)
            cmbBaudRate.SelectedItem = "921600";
            cmbDataBits.SelectedItem = "8";
            cmbStopBits.SelectedItem = "1";
            cmbParity.SelectedItem = "None";
            cmbPacketSize.SelectedItem = "16384";

            // 串口状态轮询定时器:主动检测拔线(拔线时.Net串口事件不一定会触发)
            _portWatchTimer = new System.Windows.Forms.Timer();
            _portWatchTimer.Interval = 500;
            _portWatchTimer.Tick += PortWatchTimer_Tick;

            LogMessage("系统就绪，请选择BIN文件并配置串口。");
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            StopBurn();
            CloseSerial();
        }

        private void RefreshPorts()
        {
            cmbPortName.Items.Clear();
            string[] ports = SerialPort.GetPortNames();
            Array.Sort(ports);
            cmbPortName.Items.AddRange(ports);
            if (cmbPortName.Items.Count > 0)
            {
                cmbPortName.SelectedIndex = 0;
            }
        }

        private void btnRefreshPort_Click(object sender, EventArgs e)
        {
            RefreshPorts();
            LogMessage("已刷新串口列表。");
        }

        private void btnBrowse_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog ofd = new OpenFileDialog())
            {
                ofd.Filter = "BIN文件|*.bin|所有文件|*.*";
                ofd.Title = "选择BIN固件文件";
                if (ofd.ShowDialog() == DialogResult.OK)
                {
                    txtFilePath.Text = ofd.FileName;
                    try
                    {
                        FileInfo fi = new FileInfo(ofd.FileName);
                        _fileData = File.ReadAllBytes(ofd.FileName);
                        lblFileInfo.Text = $"文件大小: {fi.Length:N0} 字节 ({fi.Length / 1024.0:F2} KB)";
                        LogMessage($"已加载文件: {ofd.FileName}, 大小: {fi.Length} 字节");
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"读取文件失败: {ex.Message}", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        LogMessage($"错误: 读取文件失败 - {ex.Message}");
                    }
                }
            }
        }

        private void btnOpenSerial_Click(object sender, EventArgs e)
        {
            if (cmbPortName.SelectedItem == null)
            {
                MessageBox.Show("请先选择串口！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            try
            {
                if (cmbPortName.SelectedItem == null || cmbBaudRate.SelectedItem == null ||
                    cmbDataBits.SelectedItem == null || cmbStopBits.SelectedItem == null ||
                    cmbParity.SelectedItem == null)
                {
                    MessageBox.Show("请先完整配置串口参数。", "提示", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

                _serialPort = new SerialPort(
                    cmbPortName.SelectedItem.ToString(),
                    int.Parse(cmbBaudRate.SelectedItem.ToString()),
                    (Parity)Enum.Parse(typeof(Parity), cmbParity.SelectedItem.ToString()),
                    int.Parse(cmbDataBits.SelectedItem.ToString()),
                    (StopBits)Enum.Parse(typeof(StopBits), cmbStopBits.SelectedItem.ToString())
                );

                _serialPort.ReadTimeout = 200;
                _serialPort.WriteTimeout = 5000;
                _serialPort.WriteBufferSize = 65536;   // 支持大包(最大16384字节)一次性写入, 避免低波特率下 Write 超时
                _serialPort.Open();
                _serialPort.DataReceived += SerialPort_DataReceived;
                _serialPort.ErrorReceived += SerialPort_ErrorReceived;
                _serialDisconnected = false;
                if (_portWatchTimer != null)
                {
                    _portWatchTimer.Start();
                }

                btnOpenSerial.Enabled = false;
                btnCloseSerial.Enabled = true;
                cmbPortName.Enabled = false;
                cmbBaudRate.Enabled = false;
                cmbDataBits.Enabled = false;
                cmbStopBits.Enabled = false;
                cmbParity.Enabled = false;
                btnRefreshPort.Enabled = false;

                lblStatus.Text = "状态:串口已打开";
                LogMessage($"串口 {cmbPortName.SelectedItem} 已打开 @ {cmbBaudRate.SelectedItem} bps");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"打开串口失败: {ex.Message}", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                LogMessage($"错误: 打开串口失败 - {ex.Message}");
            }
        }

        private void btnCloseSerial_Click(object sender, EventArgs e)
        {
            CloseSerial();
        }

        private void CloseSerial()
        {
            if (_serialPort != null && _serialPort.IsOpen)
            {
                _serialPort.DataReceived -= SerialPort_DataReceived;
                _serialPort.ErrorReceived -= SerialPort_ErrorReceived;
                _serialPort.Close();
                _serialPort.Dispose();
                _serialPort = null;
                _serialDisconnected = false;
                if (_portWatchTimer != null) _portWatchTimer.Stop();

                btnOpenSerial.Enabled = true;
                btnCloseSerial.Enabled = false;
                cmbPortName.Enabled = true;
                cmbBaudRate.Enabled = true;
                cmbDataBits.Enabled = true;
                cmbStopBits.Enabled = true;
                cmbParity.Enabled = true;
                btnRefreshPort.Enabled = true;

                lblStatus.Text = "状态:就绪";
                LogMessage("串口已关闭。");
            }
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                int bytesToRead = _serialPort.BytesToRead;
                if (bytesToRead > 0)
                {
                    byte[] data = new byte[bytesToRead];
                    _serialPort.Read(data, 0, bytesToRead);
                    ProcessIncoming(data);
                }
            }
            catch (Exception ex)
            {
                LogMessage($"[接收错误] {ex.Message}", Color.Red);
                CheckConnectionLost();
            }
        }

        // 将收到的字节累积到缓冲: 按定长帧(ACK: 6字节)逐帧提取,并兼容下位机 printfS 文本日志
        private void ProcessIncoming(byte[] chunk)
        {
            lock (_rxLock)
            {
                _rxBuffer.AddRange(chunk);

                while (_rxBuffer.Count > 0)
                {
                    byte b0 = _rxBuffer[0];

                    // 1) 二进制 ACK 帧: 帧头/第5字节均为 0x55, 且 XOR 校验通过才按帧提取;
                    //    否则回退到文本/噪声处理,避免把文本误当帧丢弃
                    if (b0 == ACK_HEADER && _rxBuffer.Count >= 6 && _rxBuffer[4] == PROTO_FOOTER)
                    {
                        byte[] frame = _rxBuffer.GetRange(0, 6).ToArray();
                        byte xor = (byte)(frame[0] ^ frame[1] ^ frame[2] ^ frame[3] ^ frame[4]);
                        if (xor == frame[5])
                        {
                            _rxBuffer.RemoveRange(0, 6);
                            ProcessReceivedData(frame);
                            continue;
                        }
                    }

                    // 2) 文本日志(下位机 printfS 经 UART1 发回,以 \r\n 结尾): 取一整行输出
                    int nl = _rxBuffer.IndexOf((byte)0x0A);   // '\n'
                    if (nl >= 0)
                    {
                        int len = nl;
                        if (len > 0 && _rxBuffer[len - 1] == 0x0D) len--;   // 去掉 '\r'
                        if (len > 0)
                        {
                            string line = Encoding.ASCII.GetString(_rxBuffer.GetRange(0, len).ToArray());
                            LogMessage($"[下位机] {line}", Color.Yellow);
                        }
                        _rxBuffer.RemoveRange(0, nl + 1);
                        continue;
                    }

                    // 3) 帧头 0x55 但不足6字节: 等待更多数据
                    if (b0 == ACK_HEADER && _rxBuffer.Count < 6)
                    {
                        return;
                    }

                    // 4) 非帧头、且尚无换行: 可能是噪声或行尾未到。
                    //    缓冲超上限仍无换行时丢弃头部字节,防止噪声无限堆积
                    if (_rxBuffer.Count >= 1024)
                    {
                        _rxBuffer.RemoveAt(0);
                        continue;
                    }
                    return;
                }
            }
        }

        private void SerialPort_ErrorReceived(object sender, SerialErrorReceivedEventArgs e)
        {
            // 设备被拔出/通信错误时,检测串口是否已从系统消失
            CheckConnectionLost();
        }

        private void PortWatchTimer_Tick(object sender, EventArgs e)
        {
            CheckConnectionLost();
        }

        // CH340等USB转串口芯片在拔线后,只要程序还持有串口句柄,COM口名就会以"幽灵端口"
        // 形式保留在系统里(实测:列表/注册表/重开探测全部仍显示在位)。
        // 唯一可靠办法:空闲时周期性"关闭->重查->重开":
        //   释放句柄后,若设备已拔出,幽灵端口立即从系统消失 -> 判定断开;
        //   若设备仍在,立即重开(对用户无感),继续正常使用。
        private void CheckConnectionLost()
        {
            if (_serialPort == null || _serialDisconnected || _probingPort) return;

            string portName;
            try
            {
                portName = _serialPort.PortName;
            }
            catch
            {
                HandleSerialDisconnected();
                return;
            }
            if (string.IsNullOrEmpty(portName)) return;

            bool inPortList = false;
            bool inRegistry = false;

            try
            {
                foreach (string p in SerialPort.GetPortNames())
                {
                    if (string.Equals(p, portName, StringComparison.OrdinalIgnoreCase))
                    {
                        inPortList = true;
                        break;
                    }
                }
            }
            catch { }

            try
            {
                using (RegistryKey key = Registry.LocalMachine.OpenSubKey(@"HARDWARE\DEVICEMAP\SERIALCOMM"))
                {
                    if (key != null)
                    {
                        foreach (string v in key.GetValueNames())
                        {
                            object val = key.GetValue(v);
                            if (val != null && string.Equals(val.ToString(), portName, StringComparison.OrdinalIgnoreCase))
                            {
                                inRegistry = true;
                                break;
                            }
                        }
                    }
                }
            }
            catch { }

            // 廉价检测:列表和注册表都查不到 -> 设备已被系统移除
            if (!inPortList && !inRegistry)
            {
                HandleSerialDisconnected();
                return;
            }

            // 幽灵端口场景:列表/注册表都还在。烧录/中止期间不探测;
            // 空闲时每约1秒做一次"关闭->重查->重开"权威探测。
            if (_isBurning || _stopRequested)
            {
                _idleProbeCounter = 0;
                return;
            }

            _idleProbeCounter++;
            if (_idleProbeCounter < 2) return;   // 500ms * 2 = 约1秒一次

            _idleProbeCounter = 0;
            _probingPort = true;
            try
            {
                // 1) 关闭句柄:释放后幽灵端口才会消失
                _serialPort.Close();
                Thread.Sleep(30);   // 等驱动完成句柄释放

                // 2) 重查:设备已拔出则端口已不在列表
                bool stillThere = false;
                try
                {
                    foreach (string p in SerialPort.GetPortNames())
                    {
                        if (string.Equals(p, portName, StringComparison.OrdinalIgnoreCase))
                        {
                            stillThere = true;
                            break;
                        }
                    }
                }
                catch { }

                if (!stillThere)
                {
                    HandleSerialDisconnected();
                    return;
                }

                // 3) 重开:设备还在,恢复占用;打开失败说明设备已拔出
                _serialPort.Open();
            }
            catch (Exception ex)
            {
                LogMessage($"[重开失败] {ex.Message}", Color.Red);
                HandleSerialDisconnected();
            }
            finally
            {
                _probingPort = false;
            }
        }

        private void HandleSerialDisconnected()
        {
            if (_serialDisconnected) return;   // 已处理过,避免重复弹窗
            _serialDisconnected = true;

            if (this.InvokeRequired)
            {
                this.Invoke((MethodInvoker)delegate { HandleSerialDisconnectedUI(); });
            }
            else
            {
                HandleSerialDisconnectedUI();
            }
        }

        private void HandleSerialDisconnectedUI()
        {
            // 清理已失效的串口对象
            try
            {
                if (_serialPort != null)
                {
                    _serialPort.DataReceived -= SerialPort_DataReceived;
                    _serialPort.ErrorReceived -= SerialPort_ErrorReceived;
                    if (_serialPort.IsOpen) _serialPort.Close();
                    _serialPort.Dispose();
                }
            }
            catch { }
            _serialPort = null;
            if (_portWatchTimer != null) _portWatchTimer.Stop();

            // 恢复界面控件状态
            btnOpenSerial.Enabled = true;
            btnCloseSerial.Enabled = false;
            cmbPortName.Enabled = true;
            cmbBaudRate.Enabled = true;
            cmbDataBits.Enabled = true;
            cmbStopBits.Enabled = true;
            cmbParity.Enabled = true;
            btnRefreshPort.Enabled = true;

            lblStatus.Text = "状态:串口已断开";
            LogMessage("串口连接已断开(设备被移除)。", Color.Red);

            RefreshPorts();   // 刷新串口列表,移除已不存在的端口

            MessageBox.Show("串口连接已断开，请检查串口线后重新连接。", "串口断开",
                MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }

        private void ProcessReceivedData(byte[] data)
        {
            // 烧录期间不逐包打印接收明细,仅非烧录(链接测试等)时保留,避免高频跨线程日志拖慢收包
            if (!_isBurning)
            {
                LogMessage($"[接收] {BitConverter.ToString(data).Replace("-", " ")}", Color.Cyan);
            }

            // ACK 帧: [HEADER(0x55)] [SEQ_L] [SEQ_H] [STATUS] [FOOTER(0x55)] [XOR]  共 6 字节
            // 注意: ACK 使用 [FOOTER][XOR] 顺序(与主机命令的 [XOR][FOOTER] 相反)
            if (data.Length >= 6 && data[0] == ACK_HEADER && data[4] == PROTO_FOOTER)
            {
                // XOR 校验: [HEADER..FOOTER] 五字节异或应等于 data[5]
                byte xor = (byte)(data[0] ^ data[1] ^ data[2] ^ data[3] ^ data[4]);

                if (xor != data[5])
                {
                    LogMessage($"[ACK] XOR 校验失败,丢弃: {BitConverter.ToString(data).Replace("-", " ")}", Color.Red);
                    return;
                }

                int seq = data[1] | (data[2] << 8);   // SEQ 小端
                int status = data[3];

                _lastAckSeq = seq;
                _lastAckStatus = status;

                if (_ackEvent != null)
                {
                    _ackEvent.Set();
                }

                switch (status)
                {
                    case ACK_STATUS_OK:
                        // 烧录期间成功ACK不逐包打印,仅在非烧录(链接测试)时显示
                        if (!_isBurning)
                        {
                            LogMessage($"[ACK] 包#{seq} 发送成功", Color.Green);
                        }
                        break;
                    case ACK_STATUS_CHECKSUM_ERR:
                        LogMessage($"[ACK] 包#{seq} 校验失败", Color.Orange);
                        break;

                    case ACK_STATUS_OTHER_ERR:
                        LogMessage($"[ACK] 包#{seq} 其他错误", Color.Red);
                        break;

                    default:
                        LogMessage($"[ACK] 包#{seq} 未知状态: 0x{status:X2}", Color.Orange);
                        break;
                }
            }
        }

        // AES-128-CBC 加密(PKCS7填充),用于烧录前对BIN加密,密文写入SD
        private static byte[] EncryptAes128Cbc(byte[] plain)
        {
            using (Aes aes = Aes.Create())
            {
                aes.Key = AES_KEY;
                aes.IV = AES_IV;
                aes.Mode = CipherMode.CBC;
                aes.Padding = PaddingMode.PKCS7;
                using (ICryptoTransform enc = aes.CreateEncryptor())
                {
                    return enc.TransformFinalBlock(plain, 0, plain.Length);
                }
            }
        }

        // CRC-16/X-25 (反射式: poly=0x8408=0x1021位反转, init=0xFFFF, 最终取反) — 与下位机 get_crc_16 一致
        // 注意: 下位机 crc.c 使用反射式查表(右移), 表值如 0x0000,0x1189,0x2312..., 此处必须与其保持一致
        private static readonly ushort[] Crc16Table = BuildCrc16Table();

        private static ushort[] BuildCrc16Table()
        {
            ushort[] table = new ushort[256];
            for (ushort i = 0; i < 256; i++)
            {
                ushort crc = i;
                for (int j = 0; j < 8; j++)
                {
                    crc = (ushort)((crc & 1) != 0 ? (crc >> 1) ^ 0x8408 : crc >> 1);
                }
                table[i] = crc;
            }
            return table;
        }

        private static ushort Crc16Ccitt(byte[] data)
        {
            ushort crc = 0xFFFF;
            foreach (byte b in data)
            {
                crc = (ushort)((crc >> 8) ^ Crc16Table[(crc ^ b) & 0xFF]);
            }
            return (ushort)~crc;
        }

        private static ushort Crc16Ccitt(byte[] data, int offset, int length)
        {
            ushort crc = 0xFFFF;
            for (int i = offset; i < offset + length; i++)
            {
                crc = (ushort)((crc >> 8) ^ Crc16Table[(crc ^ data[i]) & 0xFF]);
            }
            return (ushort)~crc;
        }

        // 构造固定命令包: [HEADER] [CMD] [XOR] [FOOTER]  (4字节)
        // XOR = HEADER ^ CMD
        private byte[] BuildCmdPacket(byte cmd)
        {
            return new byte[] { PROTO_HEADER, cmd, (byte)(PROTO_HEADER ^ cmd), PROTO_FOOTER };
        }

        private byte[] BuildStartPacket(int plainLen, byte cmd = PROTO_CMD_START)
        {
            // [HEADER][CMD][LEN_B3][LEN_B2][LEN_B1][LEN_B0][XOR][FOOTER]  共 8 字节
            // LEN = 原始明文BIN长度(大端), 供下位机定位内层CRC并去除AES填充
            byte[] p = new byte[8];
            p[0] = PROTO_HEADER;
            p[1] = cmd;
            p[2] = (byte)(plainLen >> 24);
            p[3] = (byte)(plainLen >> 16);
            p[4] = (byte)(plainLen >> 8);
            p[5] = (byte)(plainLen);
            p[6] = (byte)(PROTO_HEADER ^ cmd ^ p[2] ^ p[3] ^ p[4] ^ p[5]); // XOR
            p[7] = PROTO_FOOTER;
            return p;
        }

        private byte[] BuildEndPacket(ushort totalCrc)
        {
            // [HEADER][CMD][CRC_H][CRC_L][XOR][FOOTER]  共 6 字节
            byte[] p = new byte[6];
            p[0] = PROTO_HEADER;
            p[1] = PROTO_CMD_END;
            p[2] = (byte)(totalCrc >> 8);   // CRC_H
            p[3] = (byte)(totalCrc & 0xFF); // CRC_L
            p[4] = (byte)(PROTO_HEADER ^ PROTO_CMD_END ^ p[2] ^ p[3]); // XOR
            p[5] = PROTO_FOOTER;
            return p;
        }

        private byte[] BuildLinkTestPacket()
        {
            return BuildCmdPacket(PROTO_CMD_LINK_TEST);
        }

        private byte[] BuildLinkEndPacket()
        {
            return BuildCmdPacket(PROTO_CMD_LINK_END);
        }

        // 链接测试:发送 LINK_TEST,等待ACK,成功后发送 LINK_END 结束测试
        private bool TestLink()
        {
            if (_serialPort == null || !_serialPort.IsOpen)
            {
                LogMessage("[链接测试] 串口未打开", Color.Red);
                return false;
            }

            byte[] testPkt = BuildLinkTestPacket();
            LogMessage($"[发送] 链接测试命令: {BitConverter.ToString(testPkt).Replace("-", " ")}");

            bool ok = SendAndWaitForAck(testPkt, 0, ACK_TIMEOUT_MS, true);
            if (!ok)
            {
                LogMessage("[链接测试] 未收到响应,链接失败", Color.Red);
                return false;
            }

            if (_lastAckStatus != ACK_STATUS_OK)
            {
                LogMessage($"[链接测试] 对端返回错误状态: {_lastAckStatus}", Color.Red);
                return false;
            }

            LogMessage("[链接测试] 链接正常", Color.Green);

            byte[] endPkt = BuildLinkEndPacket();
            LogMessage($"[发送] 结束测试命令: {BitConverter.ToString(endPkt).Replace("-", " ")}");
            SendAndWaitForAck(endPkt, 0, ACK_TIMEOUT_MS, true);

            return true;
        }

        private byte[] BuildDataPacket(int seq, byte[] data, int offset, int length)
        {
            // [HEADER][CMD][SEQ_L][SEQ_H][LEN_H][LEN_L][DATA...][CRC_H][CRC_L][XOR][FOOTER]  共 10 + length 字节
            byte[] packet = new byte[10 + length];

            packet[0] = PROTO_HEADER;
            packet[1] = PROTO_CMD_DATA;
            packet[2] = (byte)(seq & 0xFF);         // SEQ_L (小端)
            packet[3] = (byte)((seq >> 8) & 0xFF);  // SEQ_H
            packet[4] = (byte)((length >> 8) & 0xFF); // LEN_H (大端)
            packet[5] = (byte)(length & 0xFF);       // LEN_L

            Buffer.BlockCopy(data, offset, packet, 6, length);

            // CRC16(对DATA),大端
            ushort crc = Crc16Ccitt(data, offset, length);
            packet[6 + length] = (byte)(crc >> 8);   // CRC_H
            packet[7 + length] = (byte)(crc & 0xFF); // CRC_L

            // XOR: 对 [HEADER..DATA] 范围内字节异或,不含 CRC/XOR/FOOTER
            byte xor = 0;
            for (int i = 0; i < 6 + length; i++)
            {
                xor ^= packet[i];
            }
            packet[8 + length] = xor;
            packet[9 + length] = PROTO_FOOTER;

            return packet;
        }

        private bool SendAndWaitForAck(byte[] packet, int expectedSeq, int timeoutMs, bool allowRetry)
        {
            _ackEvent = new AutoResetEvent(false);
            _expectedSeq = expectedSeq;

            int maxRetry = allowRetry ? MAX_RETRY : 1;

            for (int retry = 0; retry < maxRetry; retry++)
            {
                if (_stopRequested) return false;

                try
                {
                    _serialPort.DiscardInBuffer();
                    _serialPort.Write(packet, 0, packet.Length);
                    _serialPort.BaseStream.Flush();
                }
                catch (Exception ex)
                {
                    LogMessage($"[发送错误] {ex.Message}", Color.Red);
                    return false;
                }

                if (_ackEvent.WaitOne(timeoutMs))
                {
                    if (_lastAckSeq != expectedSeq)
                    {
                        LogMessage($"[警告] 收到序号#{_lastAckSeq} 期望#{expectedSeq}, 重试 {retry + 1}/{maxRetry}", Color.Orange);
                        continue;
                    }

                    if (_lastAckStatus == ACK_STATUS_OK)
                    {
                        return true;
                    }
                    else
                    {
                        LogMessage($"[失败] 包#{expectedSeq} 状态错误, 重试 {retry + 1}/{maxRetry}", Color.Red);
                        Thread.Sleep(50);
                    }
                }
                else
                {
                    LogMessage($"[超时] 包#{expectedSeq} 等待应答超时, 重试 {retry + 1}/{maxRetry}", Color.Orange);
                }
            }

            LogMessage($"[失败] 包#{expectedSeq} 重试次数已达上限", Color.Red);
            return false;
        }

        private void btnStartBurn_Click(object sender, EventArgs e)
        {
            if (_fileData == null || _fileData.Length == 0)
            {
                MessageBox.Show("请先选择有效的BIN文件！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (_serialPort == null || !_serialPort.IsOpen)
            {
                MessageBox.Show("请先打开串口！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (_isBurning) return;

            _isBurning = true;
            _stopRequested = false;
            _bytesSent = 0;

            btnStartBurn.Enabled = false;
            btnStopBurn.Enabled = true;
            btnBrowse.Enabled = false;

            progressBar.Value = 0;
            progressBar.Maximum = 100;
            lblProgress.Text = $"进度: 0 / {_fileData.Length} 字节 (0%)";
            lblStatus.Text = "状态:烧录中...";

            int packetSize = int.Parse(cmbPacketSize.SelectedItem.ToString());

            _burnThread = new Thread(() => BurnFile(packetSize));
            _burnThread.IsBackground = true;
            _burnThread.Start();
        }

        private void BurnFile(int packetSize)
        {
            var burnerStopwatch = System.Diagnostics.Stopwatch.StartNew();
            try
            {
                bool isPlainMode = false;
                this.Invoke((MethodInvoker)delegate { isPlainMode = chkPlainMode.Checked; });

                ushort totalCrc;
                byte[] burnData;

                if (isPlainMode)
                {
                    // 明文模式: 不加密, 直接发送原始BIN数据, 仅XOR+CRC校验
                    burnData = _fileData;
                    totalCrc = Crc16Ccitt(burnData);
                    LogMessage($"[明文模式] 跳过AES加密, 原始BIN {_fileData.Length} 字节, CRC16: 0x{totalCrc:X4}", Color.Cyan);
                }
                else
                {
                    // 加密模式: AES-128-CBC 加密后发送
                    // 内层CRC: 对原始明文BIN计算(解密后校验用)
                    ushort innerCrc = Crc16Ccitt(_fileData);

                    // payload = 明文BIN + 内层CRC(2字节,大端), 一起加密
                    byte[] payload = new byte[_fileData.Length + 2];
                    Buffer.BlockCopy(_fileData, 0, payload, 0, _fileData.Length);
                    payload[_fileData.Length] = (byte)(innerCrc >> 8);
                    payload[_fileData.Length + 1] = (byte)(innerCrc & 0xFF);

                    // AES-128-CBC 加密:密文写入SD(防抄录)
                    burnData = EncryptAes128Cbc(payload);
                    LogMessage($"[加密] AES-128-CBC 已启用, 明文BIN {_fileData.Length} 字节 -> 密文 {burnData.Length} 字节", Color.Cyan);

                    // 外层CRC: 对密文计算(传输完整性)
                    totalCrc = Crc16Ccitt(burnData);
                    LogMessage($"[CRC] 明文总CRC16: 0x{innerCrc:X4}, 密文总CRC16: 0x{totalCrc:X4}", Color.Cyan);
                }

                int totalBytes = burnData.Length;
                int offset = 0;
                int packetIndex = 0;

                LogMessage($"开始烧录: 总大小 {totalBytes} 字节 ({totalBytes / 1024.0 / 1024.0:F2} MB), 包大小 {packetSize} 字节");

                byte[] startPkt = BuildStartPacket(_fileData.Length, isPlainMode ? PROTO_CMD_START_PLAIN : PROTO_CMD_START);
                LogMessage($"[发送] 开始命令: {BitConverter.ToString(startPkt).Replace("-", " ")}");

                bool startAckOk = SendAndWaitForAck(startPkt, 0, ACK_TIMEOUT_MS, true);
                if (!startAckOk)
                {
                    LogMessage("烧录失败: 开始命令未收到ACK", Color.Red);
                    UpdateStatus("状态:烧录失败");
                    this.Invoke((MethodInvoker)delegate
                    {
                        MessageBox.Show("烧录失败！\n开始命令未收到ACK,请检查对端响应。", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    });
                    return;
                }

                if (_lastAckStatus != ACK_STATUS_OK)
                {
                    LogMessage($"烧录失败: 开始命令返回错误状态 {_lastAckStatus}", Color.Red);
                    UpdateStatus("状态:烧录失败");
                    this.Invoke((MethodInvoker)delegate
                    {
                        MessageBox.Show($"烧录失败！\n开始命令返回错误状态: {_lastAckStatus}", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    });
                    return;
                }

                while (offset < totalBytes && !_stopRequested)
                {
                    int bytesToSend = Math.Min(packetSize, totalBytes - offset);
                    packetIndex++;

                    byte[] pkt = BuildDataPacket(packetIndex, burnData, offset, bytesToSend);
                    // 逐包发送日志降频:首包及每20包打印一次,其余由进度条体现
                   

                    bool ackOk = SendAndWaitForAck(pkt, packetIndex, DATA_ACK_TIMEOUT_MS, false);

                    if (!ackOk)
                    {
                        LogMessage($"烧录失败: 包#{packetIndex} 未成功确认", Color.Red);
                        UpdateStatus("状态:烧录失败");
                        this.Invoke((MethodInvoker)delegate
                        {
                            MessageBox.Show($"烧录失败！\n失败数据包: #{packetIndex}\n偏移: {offset}", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        });
                        break;
                    }

                    _bytesSent += bytesToSend;
                    offset += bytesToSend;

                    UpdateProgress(_bytesSent, totalBytes);
                }

                if (_stopRequested)
                {
                    LogMessage("烧录已被用户中止。", Color.Orange);
                    UpdateStatus("状态:已中止");
                }
                else if (offset >= totalBytes)
                {
                    byte[] endPkt = BuildEndPacket(totalCrc);
                    LogMessage($"[发送] 结束命令(带总CRC): {BitConverter.ToString(endPkt).Replace("-", " ")}");

                    // 等待下位机总CRC核对结果
                    bool endAckOk = SendAndWaitForAck(endPkt, 0, ACK_TIMEOUT_MS, true);
                    if (endAckOk)
                    {
                        if (_lastAckStatus == ACK_STATUS_OK)
                        {
                            LogMessage($"[核对] 外层CRC(密文)与内层CRC(明文)均校验通过, 数据正确", Color.Green);
                        }
                        else if (_lastAckStatus == ACK_STATUS_CHECKSUM_ERR)
                        {
                            LogMessage($"[核对] 外层CRC(密文)校验失败, 传输数据损坏", Color.Red);
                        }
                        else
                        {
                            LogMessage($"[核对] 内层CRC(明文)校验失败, 解密结果不符(请检查Key/IV是否一致)", Color.Red);
                        }
                    }
                    else
                    {
                        LogMessage($"[核对] 未收到下位机总CRC核对响应", Color.Orange);
                    }

                    burnerStopwatch.Stop();
                    string modeTag = isPlainMode ? "明文" : "加密";
                    LogMessage($"烧录完成({modeTag}模式)！共发送 {_bytesSent} 字节, {packetIndex} 个数据包, 总CRC16: 0x{totalCrc:X4}, 总耗时 {burnerStopwatch.Elapsed.TotalSeconds:F2} 秒", Color.Green);
                    UpdateStatus("状态:烧录完成");
                    this.Invoke((MethodInvoker)delegate
                    {
                        MessageBox.Show($"烧录完成({modeTag}模式)！\n总字节数: {_bytesSent}\n数据包数: {packetIndex}\n总CRC16: 0x{totalCrc:X4}", "完成", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    });
                }
            }
            catch (Exception ex)
            {
                LogMessage($"烧录异常: {ex.Message}", Color.Red);
                this.Invoke((MethodInvoker)delegate
                {
                    MessageBox.Show($"烧录异常: {ex.Message}", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                });
            }
            finally
            {
                _isBurning = false;
                _stopRequested = false;
                if (_ackEvent != null)
                {
                    _ackEvent.Close();
                    _ackEvent = null;
                }
                this.Invoke((MethodInvoker)delegate
                {
                    btnStartBurn.Enabled = true;
                    btnStopBurn.Enabled = false;
                    btnBrowse.Enabled = true;
                });
            }
        }

        private void btnStopBurn_Click(object sender, EventArgs e)
        {
            if (_isBurning)
            {
                _stopRequested = true;
                if (_ackEvent != null) _ackEvent.Set();
                LogMessage("正在停止烧录...", Color.Orange);
            }
        }

        private void StopBurn()
        {
            if (_isBurning)
            {
                _stopRequested = true;
                if (_ackEvent != null) _ackEvent.Set();
                if (_burnThread != null && _burnThread.IsAlive)
                {
                    _burnThread.Join(2000);
                }
            }
        }

        private void UpdateProgress(int sent, int total)
        {
            if (this.InvokeRequired)
            {
                this.Invoke((Action<int, int>)UpdateProgress, sent, total);
                return;
            }

            int percent = (int)((double)sent / total * 100);
            if (percent > 100) percent = 100;
            progressBar.Value = percent;
            lblProgress.Text = $"进度: {sent:N0} / {total:N0} 字节 ({percent}%)";
        }

        private void UpdateStatus(string status)
        {
            if (this.InvokeRequired)
            {
                this.Invoke((Action<string>)UpdateStatus, status);
                return;
            }
            lblStatus.Text = status;
        }

        private void LogMessage(string message)
        {
            LogMessage(message, Color.Lime);
        }

        private void LogMessage(string message, Color color)
        {
            if (this.InvokeRequired)
            {
                this.Invoke((Action<string, Color>)LogMessage, message, color);
                return;
            }

            string timestamp = DateTime.Now.ToString("HH:mm:ss.fff");
            txtLog.SelectionStart = txtLog.TextLength;
            txtLog.SelectionLength = 0;
            txtLog.SelectionColor = color;
            txtLog.AppendText($"[{timestamp}] {message}\n");
            txtLog.SelectionColor = txtLog.ForeColor;
            txtLog.ScrollToCaret();
        }

        private void btnClearLog_Click(object sender, EventArgs e)
        {
            txtLog.Clear();
            LogMessage("日志已清空。");
        }

        private void txtLog_TextChanged(object sender, EventArgs e)
        {

        }

        private void groupBoxFile_Enter(object sender, EventArgs e)
        {

        }

        private void lblFileInfo_Click(object sender, EventArgs e)
        {

        }
    }
}
