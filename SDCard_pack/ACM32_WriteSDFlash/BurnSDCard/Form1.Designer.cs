namespace BurnSDCard
{
    partial class Form1
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        private void InitializeComponent()
        {
            this.groupBoxFile = new System.Windows.Forms.GroupBox();
            this.lblFileInfo = new System.Windows.Forms.Label();
            this.txtFilePath = new System.Windows.Forms.TextBox();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.lblFilePath = new System.Windows.Forms.Label();
            this.groupBoxSerial = new System.Windows.Forms.GroupBox();
            this.btnCloseSerial = new System.Windows.Forms.Button();
            this.btnOpenSerial = new System.Windows.Forms.Button();
            this.btnRefreshPort = new System.Windows.Forms.Button();
            this.cmbBaudRate = new System.Windows.Forms.ComboBox();
            this.cmbParity = new System.Windows.Forms.ComboBox();
            this.cmbStopBits = new System.Windows.Forms.ComboBox();
            this.cmbDataBits = new System.Windows.Forms.ComboBox();
            this.cmbPortName = new System.Windows.Forms.ComboBox();
            this.lblBaudRate = new System.Windows.Forms.Label();
            this.lblParity = new System.Windows.Forms.Label();
            this.lblStopBits = new System.Windows.Forms.Label();
            this.lblDataBits = new System.Windows.Forms.Label();
            this.lblPortName = new System.Windows.Forms.Label();
            this.groupBoxControl = new System.Windows.Forms.GroupBox();
            this.chkPlainMode = new System.Windows.Forms.CheckBox();
            this.lblStatus = new System.Windows.Forms.Label();
            this.lblProgress = new System.Windows.Forms.Label();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.btnStopBurn = new System.Windows.Forms.Button();
            this.btnStartBurn = new System.Windows.Forms.Button();
            this.cmbPacketSize = new System.Windows.Forms.ComboBox();
            this.lblPacketSize = new System.Windows.Forms.Label();
            this.groupBoxLog = new System.Windows.Forms.GroupBox();
            this.txtLog = new System.Windows.Forms.RichTextBox();
            this.btnClearLog = new System.Windows.Forms.Button();
            this.groupBoxFile.SuspendLayout();
            this.groupBoxSerial.SuspendLayout();
            this.groupBoxControl.SuspendLayout();
            this.groupBoxLog.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxFile
            // 
            this.groupBoxFile.Controls.Add(this.lblFileInfo);
            this.groupBoxFile.Controls.Add(this.txtFilePath);
            this.groupBoxFile.Controls.Add(this.btnBrowse);
            this.groupBoxFile.Controls.Add(this.lblFilePath);
            this.groupBoxFile.Location = new System.Drawing.Point(12, 12);
            this.groupBoxFile.Name = "groupBoxFile";
            this.groupBoxFile.Size = new System.Drawing.Size(864, 80);
            this.groupBoxFile.TabIndex = 0;
            this.groupBoxFile.TabStop = false;
            this.groupBoxFile.Text = "文件选择";
            this.groupBoxFile.Enter += new System.EventHandler(this.groupBoxFile_Enter);
            // 
            // lblFileInfo
            // 
            this.lblFileInfo.AutoSize = true;
            this.lblFileInfo.ForeColor = System.Drawing.Color.Blue;
            this.lblFileInfo.Location = new System.Drawing.Point(221, 56);
            this.lblFileInfo.Name = "lblFileInfo";
            this.lblFileInfo.Size = new System.Drawing.Size(65, 12);
            this.lblFileInfo.TabIndex = 3;
            this.lblFileInfo.Text = "未选择文件";
            this.lblFileInfo.Click += new System.EventHandler(this.lblFileInfo_Click);
            // 
            // txtFilePath
            // 
            this.txtFilePath.Location = new System.Drawing.Point(223, 20);
            this.txtFilePath.Name = "txtFilePath";
            this.txtFilePath.Size = new System.Drawing.Size(404, 21);
            this.txtFilePath.TabIndex = 1;
            // 
            // btnBrowse
            // 
            this.btnBrowse.Location = new System.Drawing.Point(643, 20);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(72, 23);
            this.btnBrowse.TabIndex = 2;
            this.btnBrowse.Text = "浏览...";
            this.btnBrowse.UseVisualStyleBackColor = true;
            this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
            // 
            // lblFilePath
            // 
            this.lblFilePath.AutoSize = true;
            this.lblFilePath.Location = new System.Drawing.Point(154, 25);
            this.lblFilePath.Name = "lblFilePath";
            this.lblFilePath.Size = new System.Drawing.Size(53, 12);
            this.lblFilePath.TabIndex = 0;
            this.lblFilePath.Text = "BIN文件:";
            // 
            // groupBoxSerial
            // 
            this.groupBoxSerial.Controls.Add(this.btnCloseSerial);
            this.groupBoxSerial.Controls.Add(this.btnOpenSerial);
            this.groupBoxSerial.Controls.Add(this.btnRefreshPort);
            this.groupBoxSerial.Controls.Add(this.cmbBaudRate);
            this.groupBoxSerial.Controls.Add(this.cmbParity);
            this.groupBoxSerial.Controls.Add(this.cmbStopBits);
            this.groupBoxSerial.Controls.Add(this.cmbDataBits);
            this.groupBoxSerial.Controls.Add(this.cmbPortName);
            this.groupBoxSerial.Controls.Add(this.lblBaudRate);
            this.groupBoxSerial.Controls.Add(this.lblParity);
            this.groupBoxSerial.Controls.Add(this.lblStopBits);
            this.groupBoxSerial.Controls.Add(this.lblDataBits);
            this.groupBoxSerial.Controls.Add(this.lblPortName);
            this.groupBoxSerial.Location = new System.Drawing.Point(12, 98);
            this.groupBoxSerial.Name = "groupBoxSerial";
            this.groupBoxSerial.Size = new System.Drawing.Size(864, 100);
            this.groupBoxSerial.TabIndex = 1;
            this.groupBoxSerial.TabStop = false;
            this.groupBoxSerial.Text = "串口配置";
            // 
            // btnCloseSerial
            // 
            this.btnCloseSerial.Enabled = false;
            this.btnCloseSerial.Location = new System.Drawing.Point(688, 20);
            this.btnCloseSerial.Name = "btnCloseSerial";
            this.btnCloseSerial.Size = new System.Drawing.Size(75, 23);
            this.btnCloseSerial.TabIndex = 17;
            this.btnCloseSerial.Text = "关闭串口";
            this.btnCloseSerial.UseVisualStyleBackColor = true;
            this.btnCloseSerial.Click += new System.EventHandler(this.btnCloseSerial_Click);
            // 
            // btnOpenSerial
            // 
            this.btnOpenSerial.Location = new System.Drawing.Point(605, 20);
            this.btnOpenSerial.Name = "btnOpenSerial";
            this.btnOpenSerial.Size = new System.Drawing.Size(75, 23);
            this.btnOpenSerial.TabIndex = 16;
            this.btnOpenSerial.Text = "打开串口";
            this.btnOpenSerial.UseVisualStyleBackColor = true;
            this.btnOpenSerial.Click += new System.EventHandler(this.btnOpenSerial_Click);
            // 
            // btnRefreshPort
            // 
            this.btnRefreshPort.Location = new System.Drawing.Point(388, 22);
            this.btnRefreshPort.Name = "btnRefreshPort";
            this.btnRefreshPort.Size = new System.Drawing.Size(60, 23);
            this.btnRefreshPort.TabIndex = 15;
            this.btnRefreshPort.Text = "刷新";
            this.btnRefreshPort.UseVisualStyleBackColor = true;
            this.btnRefreshPort.Click += new System.EventHandler(this.btnRefreshPort_Click);
            // 
            // cmbBaudRate
            // 
            this.cmbBaudRate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbBaudRate.FormattingEnabled = true;
            this.cmbBaudRate.Items.AddRange(new object[] {
            "1200",
            "2400",
            "4800",
            "9600",
            "19200",
            "38400",
            "57600",
            "115200",
            "230400",
            "460800",
            "921600"});
            this.cmbBaudRate.Location = new System.Drawing.Point(129, 60);
            this.cmbBaudRate.Name = "cmbBaudRate";
            this.cmbBaudRate.Size = new System.Drawing.Size(123, 20);
            this.cmbBaudRate.TabIndex = 10;
            // 
            // cmbParity
            // 
            this.cmbParity.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbParity.FormattingEnabled = true;
            this.cmbParity.Items.AddRange(new object[] {
            "None",
            "Odd",
            "Even",
            "Mark",
            "Space"});
            this.cmbParity.Location = new System.Drawing.Point(603, 60);
            this.cmbParity.Name = "cmbParity";
            this.cmbParity.Size = new System.Drawing.Size(77, 20);
            this.cmbParity.TabIndex = 14;
            // 
            // cmbStopBits
            // 
            this.cmbStopBits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbStopBits.FormattingEnabled = true;
            this.cmbStopBits.Items.AddRange(new object[] {
            "1",
            "1.5",
            "2"});
            this.cmbStopBits.Location = new System.Drawing.Point(463, 60);
            this.cmbStopBits.Name = "cmbStopBits";
            this.cmbStopBits.Size = new System.Drawing.Size(55, 20);
            this.cmbStopBits.TabIndex = 13;
            // 
            // cmbDataBits
            // 
            this.cmbDataBits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbDataBits.FormattingEnabled = true;
            this.cmbDataBits.Items.AddRange(new object[] {
            "5",
            "6",
            "7",
            "8"});
            this.cmbDataBits.Location = new System.Drawing.Point(321, 60);
            this.cmbDataBits.Name = "cmbDataBits";
            this.cmbDataBits.Size = new System.Drawing.Size(59, 20);
            this.cmbDataBits.TabIndex = 12;
            // 
            // cmbPortName
            // 
            this.cmbPortName.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbPortName.FormattingEnabled = true;
            this.cmbPortName.Location = new System.Drawing.Point(277, 23);
            this.cmbPortName.Name = "cmbPortName";
            this.cmbPortName.Size = new System.Drawing.Size(105, 20);
            this.cmbPortName.TabIndex = 11;
            // 
            // lblBaudRate
            // 
            this.lblBaudRate.AutoSize = true;
            this.lblBaudRate.Location = new System.Drawing.Point(68, 63);
            this.lblBaudRate.Name = "lblBaudRate";
            this.lblBaudRate.Size = new System.Drawing.Size(47, 12);
            this.lblBaudRate.TabIndex = 5;
            this.lblBaudRate.Text = "波特率:";
            // 
            // lblParity
            // 
            this.lblParity.AutoSize = true;
            this.lblParity.Location = new System.Drawing.Point(539, 63);
            this.lblParity.Name = "lblParity";
            this.lblParity.Size = new System.Drawing.Size(47, 12);
            this.lblParity.TabIndex = 9;
            this.lblParity.Text = "校验位:";
            // 
            // lblStopBits
            // 
            this.lblStopBits.AutoSize = true;
            this.lblStopBits.Location = new System.Drawing.Point(386, 63);
            this.lblStopBits.Name = "lblStopBits";
            this.lblStopBits.Size = new System.Drawing.Size(47, 12);
            this.lblStopBits.TabIndex = 8;
            this.lblStopBits.Text = "停止位:";
            // 
            // lblDataBits
            // 
            this.lblDataBits.AutoSize = true;
            this.lblDataBits.Location = new System.Drawing.Point(268, 63);
            this.lblDataBits.Name = "lblDataBits";
            this.lblDataBits.Size = new System.Drawing.Size(47, 12);
            this.lblDataBits.TabIndex = 7;
            this.lblDataBits.Text = "数据位:";
            // 
            // lblPortName
            // 
            this.lblPortName.AutoSize = true;
            this.lblPortName.Location = new System.Drawing.Point(210, 26);
            this.lblPortName.Name = "lblPortName";
            this.lblPortName.Size = new System.Drawing.Size(47, 12);
            this.lblPortName.TabIndex = 4;
            this.lblPortName.Text = "串口名:";
            // 
            // groupBoxControl
            // 
            this.groupBoxControl.Controls.Add(this.chkPlainMode);
            this.groupBoxControl.Controls.Add(this.lblStatus);
            this.groupBoxControl.Controls.Add(this.lblProgress);
            this.groupBoxControl.Controls.Add(this.progressBar);
            this.groupBoxControl.Controls.Add(this.btnStopBurn);
            this.groupBoxControl.Controls.Add(this.btnStartBurn);
            this.groupBoxControl.Controls.Add(this.cmbPacketSize);
            this.groupBoxControl.Controls.Add(this.lblPacketSize);
            this.groupBoxControl.Location = new System.Drawing.Point(12, 204);
            this.groupBoxControl.Name = "groupBoxControl";
            this.groupBoxControl.Size = new System.Drawing.Size(864, 124);
            this.groupBoxControl.TabIndex = 2;
            this.groupBoxControl.TabStop = false;
            this.groupBoxControl.Text = "烧录控制";
            // 
            // chkPlainMode
            // 
            this.chkPlainMode.AutoSize = true;
            this.chkPlainMode.Location = new System.Drawing.Point(156, 30);
            this.chkPlainMode.Name = "chkPlainMode";
            this.chkPlainMode.Size = new System.Drawing.Size(120, 16);
            this.chkPlainMode.TabIndex = 7;
            this.chkPlainMode.Text = "明文模式(不加密)";
            this.chkPlainMode.UseVisualStyleBackColor = true;
            // 
            // lblStatus
            // 
            this.lblStatus.AutoSize = true;
            this.lblStatus.Location = new System.Drawing.Point(692, 67);
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(59, 12);
            this.lblStatus.TabIndex = 6;
            this.lblStatus.Text = "状态:就绪";
            // 
            // lblProgress
            // 
            this.lblProgress.AutoSize = true;
            this.lblProgress.Location = new System.Drawing.Point(154, 92);
            this.lblProgress.Name = "lblProgress";
            this.lblProgress.Size = new System.Drawing.Size(35, 12);
            this.lblProgress.TabIndex = 5;
            this.lblProgress.Text = "进度:";
            // 
            // progressBar
            // 
            this.progressBar.Location = new System.Drawing.Point(156, 56);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(530, 23);
            this.progressBar.TabIndex = 4;
            // 
            // btnStopBurn
            // 
            this.btnStopBurn.Enabled = false;
            this.btnStopBurn.Location = new System.Drawing.Point(775, 22);
            this.btnStopBurn.Name = "btnStopBurn";
            this.btnStopBurn.Size = new System.Drawing.Size(75, 23);
            this.btnStopBurn.TabIndex = 3;
            this.btnStopBurn.Text = "停止";
            this.btnStopBurn.UseVisualStyleBackColor = true;
            this.btnStopBurn.Click += new System.EventHandler(this.btnStopBurn_Click);
            // 
            // btnStartBurn
            // 
            this.btnStartBurn.Location = new System.Drawing.Point(688, 20);
            this.btnStartBurn.Name = "btnStartBurn";
            this.btnStartBurn.Size = new System.Drawing.Size(75, 23);
            this.btnStartBurn.TabIndex = 2;
            this.btnStartBurn.Text = "开始烧录";
            this.btnStartBurn.UseVisualStyleBackColor = true;
            this.btnStartBurn.Click += new System.EventHandler(this.btnStartBurn_Click);
            // 
            // cmbPacketSize
            // 
            this.cmbPacketSize.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbPacketSize.FormattingEnabled = true;
            this.cmbPacketSize.Items.AddRange(new object[] {
            "512",
            "1024",
            "2048",
            "4096",
            "8192",
            "16384",
            "20480"});
            this.cmbPacketSize.Location = new System.Drawing.Point(63, 30);
            this.cmbPacketSize.Name = "cmbPacketSize";
            this.cmbPacketSize.Size = new System.Drawing.Size(75, 20);
            this.cmbPacketSize.TabIndex = 1;
            // 
            // lblPacketSize
            // 
            this.lblPacketSize.AutoSize = true;
            this.lblPacketSize.Location = new System.Drawing.Point(10, 33);
            this.lblPacketSize.Name = "lblPacketSize";
            this.lblPacketSize.Size = new System.Drawing.Size(47, 12);
            this.lblPacketSize.TabIndex = 0;
            this.lblPacketSize.Text = "包大小:";
            // 
            // groupBoxLog
            // 
            this.groupBoxLog.Controls.Add(this.txtLog);
            this.groupBoxLog.Controls.Add(this.btnClearLog);
            this.groupBoxLog.Location = new System.Drawing.Point(12, 334);
            this.groupBoxLog.Name = "groupBoxLog";
            this.groupBoxLog.Size = new System.Drawing.Size(865, 213);
            this.groupBoxLog.TabIndex = 3;
            this.groupBoxLog.TabStop = false;
            this.groupBoxLog.Text = "通信日志";
            // 
            // txtLog
            // 
            this.txtLog.BackColor = System.Drawing.Color.Black;
            this.txtLog.Font = new System.Drawing.Font("Consolas", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtLog.ForeColor = System.Drawing.Color.Lime;
            this.txtLog.Location = new System.Drawing.Point(0, 39);
            this.txtLog.Name = "txtLog";
            this.txtLog.ReadOnly = true;
            this.txtLog.Size = new System.Drawing.Size(850, 153);
            this.txtLog.TabIndex = 0;
            this.txtLog.Text = "";
            this.txtLog.TextChanged += new System.EventHandler(this.txtLog_TextChanged);
            // 
            // btnClearLog
            // 
            this.btnClearLog.Location = new System.Drawing.Point(742, 10);
            this.btnClearLog.Name = "btnClearLog";
            this.btnClearLog.Size = new System.Drawing.Size(75, 23);
            this.btnClearLog.TabIndex = 1;
            this.btnClearLog.Text = "清空";
            this.btnClearLog.UseVisualStyleBackColor = true;
            this.btnClearLog.Click += new System.EventHandler(this.btnClearLog_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(889, 559);
            this.Controls.Add(this.groupBoxLog);
            this.Controls.Add(this.groupBoxControl);
            this.Controls.Add(this.groupBoxSerial);
            this.Controls.Add(this.groupBoxFile);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "SD卡烧录工具 - 串口通信上位机";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBoxFile.ResumeLayout(false);
            this.groupBoxFile.PerformLayout();
            this.groupBoxSerial.ResumeLayout(false);
            this.groupBoxSerial.PerformLayout();
            this.groupBoxControl.ResumeLayout(false);
            this.groupBoxControl.PerformLayout();
            this.groupBoxLog.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxFile;
        private System.Windows.Forms.Label lblFileInfo;
        private System.Windows.Forms.TextBox txtFilePath;
        private System.Windows.Forms.Button btnBrowse;
        private System.Windows.Forms.Label lblFilePath;
        private System.Windows.Forms.GroupBox groupBoxSerial;
        private System.Windows.Forms.Button btnCloseSerial;
        private System.Windows.Forms.Button btnOpenSerial;
        private System.Windows.Forms.Button btnRefreshPort;
        private System.Windows.Forms.ComboBox cmbBaudRate;
        private System.Windows.Forms.ComboBox cmbParity;
        private System.Windows.Forms.ComboBox cmbStopBits;
        private System.Windows.Forms.ComboBox cmbDataBits;
        private System.Windows.Forms.ComboBox cmbPortName;
        private System.Windows.Forms.Label lblBaudRate;
        private System.Windows.Forms.Label lblParity;
        private System.Windows.Forms.Label lblStopBits;
        private System.Windows.Forms.Label lblDataBits;
        private System.Windows.Forms.Label lblPortName;
        private System.Windows.Forms.GroupBox groupBoxControl;
        private System.Windows.Forms.Label lblStatus;
        private System.Windows.Forms.Label lblProgress;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.Button btnStopBurn;
        private System.Windows.Forms.Button btnStartBurn;
        private System.Windows.Forms.ComboBox cmbPacketSize;
        private System.Windows.Forms.Label lblPacketSize;
        private System.Windows.Forms.GroupBox groupBoxLog;
        private System.Windows.Forms.RichTextBox txtLog;
        private System.Windows.Forms.Button btnClearLog;
        private System.Windows.Forms.CheckBox chkPlainMode;
    }
}
