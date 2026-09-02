using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Security.Cryptography;     //加解密
using ClosedXML.Excel;                  //excel



namespace ACM32检测识别仪
{
    public partial class Form1 : MetroFramework.Forms.MetroForm
    {
        private string globalFirmwareName;
        public const int download_size = 2050;                                   //一次下载量
        public const int download_data = download_size - 2;                      //一次数据量      下载量=数据量+2字节CRC
        public const int MenuOffset = 0x1A00;                                    //MENU起始地址
        public const int TableHeadOffset = 0x40;                                 //app表头起始地址
        public const int MenuHeadOffset = 0x20;                                  //MENU表头起始地址
        public const int SectorSize = 512;                                       //扇区大小
        public const int HeaderSize = 32;                                        //每个表头大小为32字节
        public const int cutname = 10;                                           //无效名字长度
        byte[] KeyBytes = { 0x2F, 0x2F, 0x62, 0x32, 0x33, 0x2E, 0x74, 0x76, 0x2F, 0x74, 0x6A, 0x79, 0x37, 0x56, 0x7A, 0x42 };
        byte[] IVBytes = { 0x00 };
        string HexPath;
        string HexName;
        byte[] buffer = new byte[1024 * 1024 * 5]; // 打开文件hex to bin缓存
        int bufferAdr; // 打开文件hex to bin缓存指针，用于计算bin的长度
        List<string> files = new List<string>(); // 用List代替string数组
        int BinCount = 1;
        string excelFilePath = "固件组成信息.xlsx";
        string txtFilePath = "固件包升级.txt";
        public Form1()
        {
            InitializeComponent();
            listBox2.SelectionMode = SelectionMode.MultiExtended;
            listBox3.SelectionMode = SelectionMode.MultiExtended;
        }
        #region CRC
        UInt16[] crc16_ccitt_table = new UInt16[]
       {
            0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
            0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
            0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
            0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
            0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
            0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
            0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
            0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
            0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
            0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
            0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
            0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
            0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
            0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
            0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
            0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
            0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
            0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
            0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
            0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
            0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
            0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
            0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
            0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
            0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
            0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
            0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
            0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
            0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
            0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
            0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
            0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
       };
        UInt16 GetCrc16(byte[] pData, int nLength)
        {
            int i = 0;
            UInt16 Crc16 = 0xffff;
            while (nLength > 0)
            {
                Crc16 = (ushort)((Crc16 >> 8) ^ crc16_ccitt_table[(Crc16 ^ pData[i]) & 0xff]);
                nLength--;
                i++;
            }
            return (ushort)~Crc16;
        }
        #endregion

        #region HextoBin
        public void HexFileRead(string filepath)
        {
            string szLine;
            bufferAdr = 0;

            if (filepath == "")
            {
                return;
            }

            FileStream fsRead = new FileStream(filepath, FileMode.OpenOrCreate, FileAccess.Read);
            StreamReader HexReader = new StreamReader(fsRead); //读取数据流
            while (true)
            {
                szLine = HexReader.ReadLine(); //读取Hex中一行
                if (szLine == null) { break; } //读取完毕，退出
                if (szLine.Substring(0, 1) == ":") //判断首字符是”:”
                {
                    if (szLine.Substring(1, 8) == "00000001") { break; } //文件结束标识
                    if ((szLine.Substring(8, 1) == "0") || (szLine.Substring(8, 1) == "1"))//直接解析数据类型标识为 : 00 和 01 的格式
                    {
                        int lineLength;
                        string hexString;
                        hexString = szLine.Substring(1, 2);
                        lineLength = Int32.Parse(hexString, System.Globalization.NumberStyles.HexNumber); // 获取一行的数据个数值

                        for (int i = 0; i < lineLength; i++) // hex转换为byte
                        {
                            hexString = szLine.Substring(i * 2 + 9, 2);
                            byte value = byte.Parse(hexString, System.Globalization.NumberStyles.HexNumber);
                            buffer[bufferAdr] = value;
                            bufferAdr++;
                        }
                    }
                }
            }
        }//从指定文件目录读取HEX文件并解析，放入缓存数组buffer中

        private void button5_Click(object sender, EventArgs e)
        {
            openFileDialog1.Title = "请选择要打开的hex文件";
            openFileDialog1.Multiselect = true;
            openFileDialog1.Filter = "HEX文件|*.hex";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                HexPath = openFileDialog1.FileName;
                HexName = Path.GetFileName(HexPath);
                metroTextBox1.Text = HexName;

            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            saveFileDialog1.Title = "请选择要保存bin文件的路径";
            saveFileDialog1.Filter = "BIN文件|*.bin|文本文件|*.txt|所有文件|*.*";
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string BinPath = saveFileDialog1.FileName; // 获得用户要保存的文件的路径
                if (string.IsNullOrEmpty(BinPath))
                {
                    return; // 如果没有选择路径，直接返回
                }

                string timeStamp = DateTime.Now.ToString("yyMMddHHmm");
                string BinName = Path.GetFileNameWithoutExtension(BinPath); // 获得了用户选择的文件名（不包含扩展名）
                string extension = Path.GetExtension(BinPath); // 获取文件扩展名
                string newFileName = $"{timeStamp}{BinName}{extension}"; // 将时间戳作为前缀添加到文件名
                string newFilePath = Path.Combine(Path.GetDirectoryName(BinPath), newFileName); // 构建新的文件路径

                metroTextBox2.Text = newFileName; // 显示新文件名
                HexFileRead(HexPath); // 读取原始文件

                // 使用新路径创建文件流
                using (FileStream fsWrite = new FileStream(newFilePath, FileMode.OpenOrCreate, FileAccess.Write))
                {
                    fsWrite.Write(buffer, 0, bufferAdr); // 写入数据
                }

                MessageBox.Show("保存成功");
            }
        }


        #endregion

        #region 选择文件与生成版本数据
        private void button2_Click(object sender, EventArgs e)
        {
            if (File.Exists("版本数据.xlsx"))
            {
                string[] filesToCheck = { "版本数据.xlsx" };
                bool isFileOpen = false;

                // 检查每个文件是否已打开
                foreach (var file in filesToCheck)
                {
                    if (IsFileLocked(new FileInfo(file)))
                    {
                        isFileOpen = true;
                        break;
                    }
                }
                // 如果任一文件已打开，显示错误信息并退出方法
                if (isFileOpen)
                {
                    MessageBox.Show("信息记录错误---原因：未关闭记录文件", "错误", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                    return;
                }
            }

            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "bin文件|*.bin";
            openFileDialog.Multiselect = true; // 是否可以多选
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                files = openFileDialog.FileNames.ToList(); // 转换为List
                listBox1.Items.Clear();
                listBox2.Items.Clear();
                CheckAndCreateVersionDataFile();
                foreach (string file in files)
                {
                    string originalFileName = Path.GetFileNameWithoutExtension(file);
                    string fileName = originalFileName.Length > cutname ? originalFileName.Substring(cutname) : originalFileName;  // 提取文件名中的有意义部分
                    FileInfo fileInfo = new FileInfo(file);
                    long fileSize = fileInfo.Length;
                    string version = GetFileVersionAndUpdate(fileName, false, false);
                    listBox1.Items.Add($"{fileName} ({fileSize} 字节) - 版本号：v{version}");
                    listBox2.Items.Add($"{fileName} - 版本号：v{version}");
                }
            }
        }
        #endregion

        #region 版本号操作
        //小更新
        private void button3_Click(object sender, EventArgs e)
        {
            if (listBox1.SelectedItem != null)
            {
                string selectedItem = listBox1.SelectedItem.ToString();
                string fileName = GetFileNameFromSelectedItem(selectedItem);
                string newVersion = GetFileVersionAndUpdate(fileName, true, false);
                UpdateListBoxItem(fileName, newVersion);
            }
        }
        //大更新
        private void button4_Click(object sender, EventArgs e)
        {
            if (listBox1.SelectedItem != null)
            {
                string selectedItem = listBox1.SelectedItem.ToString();
                string fileName = GetFileNameFromSelectedItem(selectedItem);
                string newVersion = GetFileVersionAndUpdate(fileName, false, true);
                UpdateListBoxItem(fileName, newVersion);
            }
        }
        //listbox 显示更新
        private void UpdateListBoxItem(string fileName, string newVersion)
        {
            for (int i = 0; i < listBox1.Items.Count; i++)
            {
                string item = listBox1.Items[i].ToString();
                if (item.StartsWith(fileName))
                {
                    try
                    {
                        // 分割字符串，使用严格的检查
                        string[] parts = item.Split(new char[] { '(', ')' }, StringSplitOptions.RemoveEmptyEntries);
                        if (parts.Length >= 2)
                        {
                            string fileSize = parts[1].Trim();
                            listBox1.Items[i] = $"{fileName} ({fileSize}) - 版本号：v{newVersion}";
                            listBox2.Items[i] = $"{fileName} - 版本号：v{newVersion}";
                        }
                        else
                        {
                            Console.WriteLine($"分割字符串时出错，数组长度不够：{string.Join(", ", parts)}");
                        }
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("更新 ListBox 项目时发生错误：" + ex.Message);
                    }
                    break;
                }
            }
        }
        private string GetFileNameFromSelectedItem(string selectedItem)
        {
            try
            {
                string[] parts = selectedItem.Split(' ');
                if (parts.Length > 0)
                {
                    return parts[0];
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("解析文件名时发生错误：" + ex.Message);
            }
            return string.Empty;
        }
        private string GetFileVersionAndUpdate(string fileName, bool isMinorUpdate, bool isMajorUpdate)
        {
            string version = "01.0"; // 默认版本号为01.0
            bool needToSave = false; // 标记是否需要保存 Excel

            try
            {
                // 打开 版本数据.xlsx 文件
                string filePath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "版本数据.xlsx");
                using (var workbook = new XLWorkbook(filePath))
                {
                    var worksheet = workbook.Worksheets.Worksheet(1);
                    // 查找文件名对应的单元格
                    var cell = worksheet.Column("A").CellsUsed(c => c.GetValue<string>() == fileName).FirstOrDefault();
                    if (cell != null)
                    {
                        // 文件名存在
                        int lastColumn = worksheet.Row(cell.Address.RowNumber).CellsUsed().Last().Address.ColumnNumber;
                        var lastVersionCell = worksheet.Cell(cell.Address.RowNumber, lastColumn);

                        if (!lastVersionCell.IsEmpty())
                        {
                            string cellValue = lastVersionCell.GetString();
                            // 从单元格值中提取版本号
                            string[] parts = cellValue.Split(new[] { " - " }, StringSplitOptions.None);
                            if (parts.Length >= 1)
                            {
                                version = parts[0]; // 提取版本号
                            }

                            // 如果是更新操作，更新版本号
                            if (isMinorUpdate || isMajorUpdate)
                            {
                                // 确保版本号格式正确
                                if (!version.Contains("."))
                                {
                                    version = version + ".0";
                                }

                                string[] versionParts = version.Split('.');
                                if (versionParts.Length != 2)
                                {
                                    MessageBox.Show($"版本号格式不正确：{version}", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                    return version;
                                }

                                // 尝试将版本号的主版本和次版本转换为整数
                                bool majorParsed = int.TryParse(versionParts[0], out int major);
                                bool minorParsed = int.TryParse(versionParts[1], out int minor);

                                if (!majorParsed || !minorParsed)
                                {
                                    MessageBox.Show($"无法解析版本号：{version}", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                    return version;
                                }

                                // 根据更新类型更新版本号
                                if (isMinorUpdate)
                                {
                                    if (minor < 9)
                                    {
                                        minor += 1;
                                    }
                                    else
                                    {
                                        MessageBox.Show("更新失败--小版本溢出", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                        return version;
                                    }
                                }
                                else if (isMajorUpdate)
                                {
                                    if (major < 99)
                                    {
                                        major += 1;
                                        minor = 0;
                                    }
                                    else
                                    {
                                        MessageBox.Show("更新失败--大版本溢出", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                        return version;
                                    }
                                }

                                version = $"{major:D2}.{minor}";

                                // 将新版本号和当前时间写入同一个单元格
                                string newCellValue = $"{version} - {DateTime.Now:yyyy-MM-dd HH:mm:ss}";
                                worksheet.Cell(cell.Address.RowNumber, lastColumn + 1).Value = newCellValue;

                                needToSave = true; // 标记需要保存
                            }
                        }
                    }
                    else
                    {
                        // 文件名不存在，添加文件名和初始版本号（仅在首次打开时）
                        int lastRow = worksheet.LastRowUsed()?.RowNumber() ?? 0;
                        int newRow = lastRow + 1;
                        worksheet.Cell(newRow, 1).Value = fileName;

                        // 无论是否为更新操作，首次记录时都需要写入版本号和时间
                        string newCellValue = $"{version} - {DateTime.Now:yyyy-MM-dd HH:mm:ss}";
                        worksheet.Cell(newRow, 2).Value = newCellValue;

                        needToSave = true; // 标记需要保存
                    }

                    // 如果需要保存，则保存工作簿
                    if (needToSave)
                    {
                        // 设置行高和列宽（可选）
                        worksheet.Rows().Height = 20; // 设置所有行高为20磅
                        worksheet.Columns().Width = 25; // 设置所有列宽为25磅

                        // 保存更改
                        workbook.Save();
                    }
                }
            }
            catch (Exception ex)
            {
                // 处理异常
                MessageBox.Show("发生错误：" + ex.Message, "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return version;
        }





        private void CheckAndCreateVersionDataFile()
        {
            if (!File.Exists("版本数据.xlsx"))
            {
                // 创建一个新的工作簿
                var workbook = new XLWorkbook();
                // 添加一个工作表
                var worksheet = workbook.Worksheets.Add("Sheet1");

                // 设置表头
                worksheet.Cell(1, 1).Value = "文件名";
                worksheet.Cell(1, 2).Value = "版本号";

                // 设置行高和列宽
                worksheet.Rows().Height = 30; // 设置所有行高为30磅
                worksheet.Columns().Width = 15; // 设置所有列宽为15磅

                // 保存工作簿
                workbook.SaveAs(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "版本数据.xlsx"));
            }
        }
        #endregion

        #region 文件写入与加密 拆分+CRC
        private void button1_Click(object sender, EventArgs e)
        {
            BinCount = 1;
            if (File.Exists("固件组成信息.xlsx"))
            {
                string[] filesToCheck = { "固件组成信息.xlsx" };
                bool isFileOpen = false;

                // 检查每个文件是否已打开
                foreach (var file in filesToCheck)
                {
                    if (IsFileLocked(new FileInfo(file)))
                    {
                        isFileOpen = true;
                        break;
                    }
                }
                // 如果任一文件已打开，显示错误信息并退出方法
                if (isFileOpen)
                {
                    MessageBox.Show("信息记录错误---原因：未关闭记录文件", "错误", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                    return;
                }
            }
            KeyBytes = ByteUtil.PadRight(KeyBytes, 16, 0x00);
            IVBytes = ByteUtil.PadLeft(IVBytes, 16, 0x00);
            // 设置文件保存对话框，只显示bin文件
            saveFileDialog1.Filter = "bin文件|*.bin";
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                // 获取当前时间，并格式化为 "yyMMddHHmm" 的形式
                string dateTimePrefix = DateTime.Now.ToString("yyMMddHHmm00");
                // 获取用户选择的文件名，并添加时间前缀
                string originalFileName = saveFileDialog1.FileName;
                string mergedBinFileName = Path.Combine(Path.GetDirectoryName(originalFileName), dateTimePrefix + Path.GetFileName(originalFileName));

                string fileNameWithoutExtension = Path.GetFileNameWithoutExtension(mergedBinFileName); // 去掉扩展名
                int currentOffset = 0; 
                try
                {
                    using (var stream = new FileStream(mergedBinFileName, FileMode.Create))
                    {
                        // 1. 获取到bin文件的数量值，直接放入到地址0x00的位置，并在后面补0，凑齐32bvte
                        stream.WriteByte((byte)(files.Count));
                        List<string> components = new List<string>();
                        // 初始化进度条
                        metroProgressBar1.Minimum = 0;
                        metroProgressBar1.Maximum = 100; // 设置最大值为100表示百分比
                        metroProgressBar1.Value = 0;
                        SetProcessValue("正在合并文件", 0, files.Count);
                        int fileIndex = 0; // 用于跟踪已处理的文件数
                                           // 处理menu文件
                        foreach (var file in files)
                        {
                            string cutfileName = Path.GetFileNameWithoutExtension(file);
                            string fileName = cutfileName.Length > cutname ? cutfileName.Substring(cutname) : cutfileName;
                            if (fileName == "MENU")
                            {
                                using (FileStream fs = new FileStream(file, FileMode.Open))
                                {
                                    byte[] buffer = new byte[fs.Length];
                                    fs.Read(buffer, 0, buffer.Length);
                                    // 获取计算好的版本号
                                    string versionStr = GetFileVersionAndUpdate(fileName, false, false);
                                    components.Add($"{cutfileName}{versionStr}");
                                    byte[] version = Encoding.UTF8.GetBytes(versionStr);
                                    stream.Seek(MenuHeadOffset, SeekOrigin.Begin);
                                    // 写入MENU文件头
                                    byte[] DataName = new byte[16];
                                    byte[] menuFileNameBytes = Encoding.UTF8.GetBytes(fileName);
                                    Array.Clear(DataName, 0, DataName.Length);
                                    Array.Copy(menuFileNameBytes, DataName, Math.Min(menuFileNameBytes.Length, 16));
                                    byte[] DataStartAddr = new byte[4];
                                    Buffer.BlockCopy(BitConverter.GetBytes(MenuOffset), 0, DataStartAddr, 0, sizeof(int));
                                    byte[] DataEndAddr = new byte[4];
                                    int menuEndAddr = MenuOffset + buffer.Length;
                                    Buffer.BlockCopy(BitConverter.GetBytes(menuEndAddr), 0, DataEndAddr, 0, sizeof(int));
                                    // 2. 识别bin文件，获取到原始表头数据(每组原始表头数据总长度28)
                                    byte[] TableHeadData_tmp = version.Concat(DataName).Concat(DataStartAddr).Concat(DataEndAddr).ToArray();
                                    UInt16 menuCrc16 = GetCrc16(TableHeadData_tmp, TableHeadData_tmp.Length);
                                    byte[] DataCrc = new byte[4];
                                    DataCrc[2] = (byte)(menuCrc16 >> 8);
                                    DataCrc[3] = (byte)(menuCrc16);
                                    // 3. 算原始表头的CRC值(4位)，计算完成后与原始表头数据拼接，拼接完的总长度为32
                                    byte[] TableHead = TableHeadData_tmp.Concat(DataCrc).ToArray();
                                    stream.Write(TableHead, 0, TableHead.Length);
                                    // 4. 计算一个bin文件的CRC值完成后与原bin文件进行拼接
                                    stream.Seek(MenuOffset, SeekOrigin.Begin);
                                    byte[] CRC = new byte[2];
                                    CRC[0] = (byte)(GetCrc16(buffer, buffer.Length) >> 8);
                                    CRC[1] = (byte)GetCrc16(buffer, buffer.Length);
                                    byte[] writebuf = buffer.Concat(CRC).ToArray();
                                    stream.Write(writebuf, 0, writebuf.Length);

                                    currentOffset = MenuOffset + writebuf.Length;  // 更新currentOffset至MENU写入结束处                                      
                                    currentOffset = ((currentOffset + SectorSize - 1) / SectorSize) * SectorSize; // 跳转至下一个扇区开始
                                    // 更新进度条和百分比显示
                                    fileIndex++;
                                    SetProcessValue("正在合并文件", fileIndex, files.Count);
                                }
                                break;
                            }
                        }
                        // 处理其他文件
                        foreach (var file in files)
                        {
                            // string fileName = Path.GetFileNameWithoutExtension(file);
                            string cutfileName = Path.GetFileNameWithoutExtension(file);
                            string fileName = cutfileName.Length > cutname ? cutfileName.Substring(cutname) : cutfileName;
                            if (fileName != "MENU")
                            {
                                using (FileStream fs = new FileStream(file, FileMode.Open))
                                {
                                    byte[] buffer = new byte[fs.Length];
                                    fs.Read(buffer, 0, buffer.Length);
                                    string versionStr = GetFileVersionAndUpdate(fileName, false, false);
                                    byte[] version = Encoding.UTF8.GetBytes(versionStr);  // 获取版本号
                                    components.Add($"{cutfileName}{versionStr}");
                                    byte[] CRC = new byte[2];
                                    CRC[0] = (byte)(GetCrc16(buffer, buffer.Length) >> 8);
                                    CRC[1] = (byte)GetCrc16(buffer, buffer.Length);
                                    byte[] writebuf = buffer.Concat(CRC).ToArray();
                                    // 写入app文件头信息到表头
                                    stream.Seek(TableHeadOffset + (BinCount - 1) * HeaderSize, SeekOrigin.Begin);
                                    byte[] DataName = new byte[16];
                                    byte[] fileNameBytes = Encoding.UTF8.GetBytes(fileName);
                                    Array.Clear(DataName, 0, DataName.Length);
                                    Array.Copy(fileNameBytes, DataName, Math.Min(fileNameBytes.Length, 16));
                                    byte[] DataStartAddr = BitConverter.GetBytes(currentOffset); 
                                    byte[] DataEndAddr = BitConverter.GetBytes(currentOffset + buffer.Length);  
                                    byte[] TableHeadData = version.Concat(DataName).Concat(DataStartAddr).Concat(DataEndAddr).ToArray();
                                    UInt16 appCrc16 = GetCrc16(TableHeadData, TableHeadData.Length);
                                    byte[] DataCrc = new byte[4]; // 4位CRC，前两位填0
                                    DataCrc[2] = (byte)(appCrc16 >> 8);
                                    DataCrc[3] = (byte)(appCrc16);
                                    byte[] TableHead = TableHeadData.Concat(DataCrc).ToArray();
                                    stream.Write(TableHead, 0, TableHead.Length);
                                    // 写入app文件及其CRC，不加偏移量
                                    stream.Seek(currentOffset, SeekOrigin.Begin);
                                    stream.Write(writebuf, 0, writebuf.Length);
                                    // 更新currentOffset至app写入结束处
                                    currentOffset += writebuf.Length;
                                    // 跳转至下一个扇区开始
                                    currentOffset = ((currentOffset + SectorSize - 1) / SectorSize) * SectorSize;
                                    BinCount++;
                                    fileIndex++;
                                    SetProcessValue("正在合并文件", fileIndex, files.Count);
                                }
                            }
                        }

                        // 6. 计算并写入文件的CRC
                        stream.Seek(0, SeekOrigin.Begin);
                        byte[] bigFileBuffer = new byte[stream.Length];
                        stream.Read(bigFileBuffer, 0, bigFileBuffer.Length);
                        UInt16 finalCrc = GetCrc16(bigFileBuffer, bigFileBuffer.Length);
                        byte[] finalCrcBytes = new byte[2];
                        finalCrcBytes[0] = (byte)(finalCrc >> 8);
                        finalCrcBytes[1] = (byte)(finalCrc);
                        stream.Seek(0, SeekOrigin.End);
                        stream.Write(finalCrcBytes, 0, finalCrcBytes.Length);

                        //7.加密
                        // 计算文件总大小
                        FileInfo info = new FileInfo(mergedBinFileName);
                        int fileSize = (int)info.Length;
                        // 获取文件内容并加密
                        stream.Seek(0, SeekOrigin.Begin);
                        byte[] rawData = new byte[stream.Length];
                        stream.Read(rawData, 0, rawData.Length);
                        // AES加密文件内容
                        byte[] encryptedData = ByteUtil.AESEncrypt(KeyBytes, IVBytes, rawData, CipherMode.CBC);
                        // 写入加密后的数据
                        stream.SetLength(0); // 清空文件内容
                        stream.Write(encryptedData, 0, encryptedData.Length);
                        stream.Flush();

                        // 8.拆分文件并添加CRC
                        List<byte[]> dataPackets = SplitAndAddCrc(encryptedData.ToArray());
                        byte[] finalData = dataPackets.SelectMany(b => b).ToArray();
                        stream.SetLength(0); // 清空文件
                        stream.Write(finalData, 0, finalData.Length);

                        // 9.保存合并信息到Excel---固件组成信息
                        SaveToExcel(fileNameWithoutExtension, components);
                        MessageBox.Show("固件包生成成功");
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine("发生错误：" + ex.Message);
                }
            }
        }

        private List<byte[]> SplitAndAddCrc(byte[] data)
        {
            List<byte[]> dataPackets = new List<byte[]>();
            int totalChunks = (data.Length + download_data - 1) / download_data; // 计算需要的块数

            for (int i = 0; i < totalChunks; i++)
            {
                byte[] packet = new byte[download_data];
                Array.Copy(data, i * download_data, packet, 0, Math.Min(download_data, data.Length - i * download_data));

                // 计算CRC
                UInt16 packetCrc = GetCrc16(packet, packet.Length);
                // 将CRC添加到数据包末尾
                byte[] packetWithCrc = new byte[download_size];
                Array.Copy(packet, packetWithCrc, packet.Length);
                packetWithCrc[download_size - 2] = (byte)(packetCrc >> 8);
                packetWithCrc[download_size - 1] = (byte)(packetCrc);

                dataPackets.Add(packetWithCrc);
            }

            return dataPackets;
        }
        #endregion

        #region 判断文件是否打开
        private bool IsFileLocked(FileInfo file)
        {
            FileStream stream = null;

            try
            {
                stream = file.Open(FileMode.Open, FileAccess.ReadWrite, FileShare.None);
            }
            catch (IOException)
            {
                // 如果无法打开文件，说明文件被占用
                return true;
            }
            finally
            {
                stream?.Close();
            }

            return false;
        }
        #endregion

        #region 生成件组成信息
        private void SaveToExcel(string firmwareName, List<string> components)
        {
            globalFirmwareName = firmwareName;  // 设置全局变量
            XLWorkbook workbook;
            IXLWorksheet worksheet;
            if (File.Exists(excelFilePath))
            {
                workbook = new XLWorkbook(excelFilePath);
                worksheet = workbook.Worksheet(1);
            }
            else
            {
                workbook = new XLWorkbook();
                worksheet = workbook.Worksheets.Add("Sheet1");
                worksheet.Cell(1, 1).Value = "固件名";
                worksheet.Cell(1, 2).Value = "组成";
            }

            var lastRow = worksheet.LastRowUsed().RowNumber();
            var existingRow = worksheet.RowsUsed().FirstOrDefault(row => row.Cell(1).GetString().Equals(firmwareName, StringComparison.OrdinalIgnoreCase));
            if (existingRow != null)
            {
                existingRow.Delete();
                lastRow--;
            }
            worksheet.Cell(lastRow + 1, 1).Value = firmwareName;
            for (int i = 0; i < components.Count; i++)
            {
                worksheet.Cell(lastRow + 1, i + 2).Value = components[i];
            }
            worksheet.Cell(lastRow + 1, components.Count + 3).Value = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");

            foreach (var row in worksheet.RowsUsed())
            {
                row.Height = 30;
            }
            foreach (var column in worksheet.ColumnsUsed())
            {
                column.Width = 23;
            }

            workbook.SaveAs(excelFilePath);

        }
        #endregion

        #region 进度条显示
        public void SetProcessValue(string TitleName, int currentStep, int totalSteps)
        {
            // 更新窗口标题为传入的TitleName
            this.Text = TitleName;

            // 计算实际的进度条值
            int value = (int)(((double)currentStep / totalSteps) * 100); // 确保计算结果为百分比

            // 确保值在进度条的最小和最大范围内
            value = Math.Max(metroProgressBar1.Minimum, Math.Min(value, metroProgressBar1.Maximum));

            // 使用 Invoke 方法确保跨线程操作，适用于WinForms
            if (this.InvokeRequired)
            {
                this.Invoke(new Action(() => {
                    metroProgressBar1.Value = value;
                    metroProgressBar1.Refresh(); // 强制刷新进度条以显示最新的值
                }));
            }
            else
            {
                metroProgressBar1.Value = value;
                metroProgressBar1.Refresh(); // 强制刷新进度条以显示最新的值
            }
        }
        #endregion

        #region 生成固件升级指令
        private void button8_Click(object sender, EventArgs e)
        {
            // 遍历 ListBox2 中所有选中的项
            foreach (var item in listBox2.SelectedItems)
            {
                string selectedItem = item.ToString();

                // 检查是否已经添加到 ListBox3，避免重复
                if (!listBox3.Items.Contains(selectedItem))
                {
                    listBox3.Items.Add(selectedItem);
                }
            }
        }
        private void button9_Click(object sender, EventArgs e)
        {
            // 从后向前遍历所有选中的项以安全删除
            for (int i = listBox3.Items.Count - 1; i >= 0; i--)
            {
                if (listBox3.GetSelected(i))
                {
                    listBox3.Items.RemoveAt(i);
                }
            }
        }

        private void button7_Click(object sender, EventArgs e)
        {
            // 检查 ListBox3 是否为空
            if (listBox3.Items.Count == 0)
            {
                MessageBox.Show("需要使用的文件为空，请添加数据后再试。", "提示", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                return; // 终止方法的进一步执行
            }

            using (StreamWriter sw = new StreamWriter(txtFilePath, true))
            {
                if (metroRadioButton2.Checked)
                {
                    StringBuilder firmwareCommands = new StringBuilder();

                    // 特别处理 MENU 项
                    var menuItems = listBox3.Items.Cast<string>().Where(item => item.StartsWith("MENU"));
                    var nonMenuItems = listBox3.Items.Cast<string>().Where(item => !item.StartsWith("MENU"));

                    foreach (var item in menuItems.Concat(nonMenuItems))
                    {
                        var parts = item.Split(new string[] { " - 版本号：v" }, StringSplitOptions.None);
                        if (parts.Length == 2)
                        {
                            string fileName = parts[0];
                            string version = parts[1];

                            // 构建烧录指令，累加每个文件名和版本号
                            firmwareCommands.Append($"{fileName}{version},");
                        }
                    }

                    // 移除最后一个逗号
                    if (firmwareCommands.Length > 0)
                        firmwareCommands.Remove(firmwareCommands.Length - 1, 1);

                    // 格式化输出到文件
                    sw.WriteLine($"固件名为{globalFirmwareName}  烧录指令为：UPDATE_SERIES {firmwareCommands.ToString()}");
                }
                else if (metroRadioButton3.Checked)
                {
                    List<string> fileNames = new List<string>();
                    // 特别处理 MENU 项
                    var menuItems = listBox3.Items.Cast<string>().Where(item => item.StartsWith("MENU"));
                    var nonMenuItems = listBox3.Items.Cast<string>().Where(item => !item.StartsWith("MENU"));

                    foreach (var item in menuItems.Concat(nonMenuItems))
                    {
                        var parts = item.Split(new string[] { " - 版本号：v" }, StringSplitOptions.None);
                        if (parts.Length > 0)
                        {
                            fileNames.Add(parts[0]);
                        }
                    }

                    // 生成最终的文件名字符串，并在最后添加分号
                    string finalOutput = string.Join(",", fileNames) + ";";

                    // 格式化输出到文件
                    sw.WriteLine($"固件名为{globalFirmwareName}  烧录指令为：ADD_SERIES {finalOutput}");
                }
                else
                {
                    MessageBox.Show("生成指令失败，请选择生成指令模式", "提示", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                    return;
                }
            }

            MessageBox.Show("文本文件已生成并保存！");
        }
        private void listBox2_DoubleClick(object sender, EventArgs e)
        {
            // 确保有选中的项
            if (listBox2.SelectedIndex != -1)
            {
                // 获取当前选中的项
                string selectedItem = listBox2.SelectedItem.ToString();

                // 检查ListBox3是否已经包含这个项
                if (!listBox3.Items.Contains(selectedItem))
                {
                    // 如果不包含，则添加到ListBox3
                    listBox3.Items.Add(selectedItem);
                }
            }
        }
        private void listBox2_KeyDown(object sender, KeyEventArgs e)
        {
            // 检查是否按下了 Ctrl 键和 A 键
            if (e.Control && e.KeyCode == Keys.A)
            {
                // 防止声音响起
                e.SuppressKeyPress = true;

                // 循环选择所有项
                for (int i = 0; i < listBox2.Items.Count; i++)
                {
                    listBox2.SetSelected(i, true);
                }
            }
        }
        #endregion

        private void saveFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }
        private void Form1_Load(object sender, EventArgs e)
        {

        }
        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }
        private void metroTextBox2_Click(object sender, EventArgs e)
        {

        }
        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
        private void metroTabPage1_Click(object sender, EventArgs e)
        {

        }

        private void metroTextBox1_Click(object sender, EventArgs e)
        {

        }

        private void metroProgressBar1_Click(object sender, EventArgs e)
        {

        }

        private void metroRadioButton2_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void listBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void metroTabPage3_Click(object sender, EventArgs e)
        {

        }
        private void Time123_TextChanged(object sender, EventArgs e)
        {
            this.Time123.Text = DateTime.Now.ToString();
        }
        private void timer2_Tick(object sender, EventArgs e)
        {
            this.Time123.Text = DateTime.Now.ToString();
        }

        private void metroRadioButton3_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void metroRadioButton2_CheckedChanged_1(object sender, EventArgs e)
        {

        }

        private void listBox3_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void groupBox2_Enter(object sender, EventArgs e)
        {

        }
    }
    public static class ByteUtil
    {
        public static byte[] PadRight(this IEnumerable<byte> SourceBytes, int TotalLength, byte PaddingByte)
        {
            if (TotalLength <= SourceBytes.Count()) return SourceBytes.ToArray();
            var TargetBytes = new byte[TotalLength];
            for (int i = 0; i < SourceBytes.Count(); i++)
                TargetBytes[i] = SourceBytes.ElementAt(i);
            for (int i = SourceBytes.Count(); i < TotalLength; i++)
                TargetBytes[i] = PaddingByte;
            return TargetBytes;
        }

        public static byte[] PadLeft(this IEnumerable<byte> SourceBytes, int TotalLength, byte PaddingByte)
        {
            if (TotalLength <= SourceBytes.Count()) return SourceBytes.ToArray();
            var TargetBytes = new byte[TotalLength];
            for (int i = 0; i < TotalLength - SourceBytes.Count(); i++)
                TargetBytes[i] = PaddingByte;
            for (int i = 0; i < SourceBytes.Count(); i++)
                TargetBytes[TotalLength - SourceBytes.Count() + i] = SourceBytes.ElementAt(i);
            return TargetBytes;
        }
        public static byte[] AESEncrypt(byte[] KeyBytes, byte[] IVBytes, byte[] EncryptBytes, CipherMode EncryptMode)
        {
            AesCryptoServiceProvider Aes = new AesCryptoServiceProvider();

            Aes.Key = KeyBytes;
            Aes.IV = IVBytes;
            Aes.Mode = EncryptMode;
            Aes.Padding = PaddingMode.Zeros;

            return Aes.CreateEncryptor().TransformFinalBlock(EncryptBytes, 0, EncryptBytes.Length);
        }


    }
}
