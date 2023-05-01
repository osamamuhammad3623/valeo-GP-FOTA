# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'gui.ui'
#
# Created by: PyQt5 UI code generator 5.15.9
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets
from app_slots import show_image_info, load_image_bin, sign_process, upload_process



class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1059, 819)
        palette = QtGui.QPalette()
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.WindowText, brush)
        brush = QtGui.QBrush(QtGui.QColor(240, 230, 230))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.Button, brush)
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 255))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.Light, brush)
        brush = QtGui.QBrush(QtGui.QColor(247, 242, 242))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.Midlight, brush)
        brush = QtGui.QBrush(QtGui.QColor(120, 115, 115))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.Dark, brush)
        brush = QtGui.QBrush(QtGui.QColor(160, 153, 153))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.Mid, brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.Text, brush)
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 255))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.BrightText, brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.ButtonText, brush)
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 255))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.Base, brush)
        brush = QtGui.QBrush(QtGui.QColor(240, 230, 230))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.Window, brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.Shadow, brush)
        brush = QtGui.QBrush(QtGui.QColor(247, 242, 242))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.AlternateBase, brush)
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 220))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.ToolTipBase, brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.ToolTipText, brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0, 128))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.PlaceholderText, brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.WindowText, brush)
        brush = QtGui.QBrush(QtGui.QColor(240, 230, 230))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.Button, brush)
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 255))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.Light, brush)
        brush = QtGui.QBrush(QtGui.QColor(247, 242, 242))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.Midlight, brush)
        brush = QtGui.QBrush(QtGui.QColor(120, 115, 115))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.Dark, brush)
        brush = QtGui.QBrush(QtGui.QColor(160, 153, 153))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.Mid, brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.Text, brush)
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 255))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.BrightText, brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.ButtonText, brush)
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 255))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.Base, brush)
        brush = QtGui.QBrush(QtGui.QColor(240, 230, 230))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.Window, brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.Shadow, brush)
        brush = QtGui.QBrush(QtGui.QColor(247, 242, 242))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.AlternateBase, brush)
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 220))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.ToolTipBase, brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.ToolTipText, brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0, 128))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Inactive, QtGui.QPalette.PlaceholderText, brush)
        brush = QtGui.QBrush(QtGui.QColor(120, 115, 115))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.WindowText, brush)
        brush = QtGui.QBrush(QtGui.QColor(240, 230, 230))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.Button, brush)
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 255))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.Light, brush)
        brush = QtGui.QBrush(QtGui.QColor(247, 242, 242))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.Midlight, brush)
        brush = QtGui.QBrush(QtGui.QColor(120, 115, 115))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.Dark, brush)
        brush = QtGui.QBrush(QtGui.QColor(160, 153, 153))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.Mid, brush)
        brush = QtGui.QBrush(QtGui.QColor(120, 115, 115))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.Text, brush)
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 255))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.BrightText, brush)
        brush = QtGui.QBrush(QtGui.QColor(120, 115, 115))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.ButtonText, brush)
        brush = QtGui.QBrush(QtGui.QColor(240, 230, 230))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.Base, brush)
        brush = QtGui.QBrush(QtGui.QColor(240, 230, 230))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.Window, brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.Shadow, brush)
        brush = QtGui.QBrush(QtGui.QColor(240, 230, 230))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.AlternateBase, brush)
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 220))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.ToolTipBase, brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.ToolTipText, brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0, 128))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Disabled, QtGui.QPalette.PlaceholderText, brush)
        MainWindow.setPalette(palette)
        font = QtGui.QFont()
        font.setPointSize(10)
        MainWindow.setFont(font)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.header = QtWidgets.QLabel(self.centralwidget)
        self.header.setGeometry(QtCore.QRect(10, 10, 1031, 41))
        font = QtGui.QFont()
        font.setPointSize(18)
        font.setBold(True)
        font.setWeight(75)
        self.header.setFont(font)
        self.header.setAlignment(QtCore.Qt.AlignCenter)
        self.header.setObjectName("header")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(50, 130, 131, 21))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.label.setFont(font)
        self.label.setObjectName("label")
        self.urgency = QtWidgets.QCheckBox(self.centralwidget)
        self.urgency.setGeometry(QtCore.QRect(330, 120, 141, 41))
        self.urgency.setObjectName("urgency")
        self.image_file_1 = QtWidgets.QPushButton(self.centralwidget)
        self.image_file_1.setGeometry(QtCore.QRect(50, 280, 141, 41))
        self.image_file_1.setObjectName("image_file_1")
        self.image_path_1 = QtWidgets.QLabel(self.centralwidget)
        self.image_path_1.setGeometry(QtCore.QRect(200, 280, 351, 41))
        font = QtGui.QFont()
        font.setPointSize(8)
        self.image_path_1.setFont(font)
        self.image_path_1.setText("")
        self.image_path_1.setObjectName("image_path_1")
        self.label_6 = QtWidgets.QLabel(self.centralwidget)
        self.label_6.setGeometry(QtCore.QRect(80, 340, 91, 21))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.label_6.setFont(font)
        self.label_6.setObjectName("label_6")
        self.image_root_index_1 = QtWidgets.QSpinBox(self.centralwidget)
        self.image_root_index_1.setGeometry(QtCore.QRect(200, 330, 71, 41))
        self.image_root_index_1.setMinimum(1)
        self.image_root_index_1.setMaximum(2)
        self.image_root_index_1.setProperty("value", 1)
        self.image_root_index_1.setObjectName("image_root_index_1")
        self.image_target_1 = QtWidgets.QComboBox(self.centralwidget)
        self.image_target_1.setGeometry(QtCore.QRect(410, 330, 141, 41))
        self.image_target_1.setObjectName("image_target_1")
        self.image_target_1.addItem("")
        self.image_target_1.addItem("")
        self.image_target_1.addItem("")
        self.label_8 = QtWidgets.QLabel(self.centralwidget)
        self.label_8.setGeometry(QtCore.QRect(340, 340, 61, 21))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.label_8.setFont(font)
        self.label_8.setObjectName("label_8")
        self.label_9 = QtWidgets.QLabel(self.centralwidget)
        self.label_9.setGeometry(QtCore.QRect(80, 470, 91, 21))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.label_9.setFont(font)
        self.label_9.setObjectName("label_9")
        self.image_target_2 = QtWidgets.QComboBox(self.centralwidget)
        self.image_target_2.setGeometry(QtCore.QRect(410, 460, 141, 41))
        self.image_target_2.setObjectName("image_target_2")
        self.image_target_2.addItem("")
        self.image_target_2.addItem("")
        self.image_target_2.addItem("")
        self.image_path_2 = QtWidgets.QLabel(self.centralwidget)
        self.image_path_2.setGeometry(QtCore.QRect(200, 410, 351, 41))
        font = QtGui.QFont()
        font.setPointSize(8)
        self.image_path_2.setFont(font)
        self.image_path_2.setText("")
        self.image_path_2.setObjectName("image_path_2")
        self.label_10 = QtWidgets.QLabel(self.centralwidget)
        self.label_10.setGeometry(QtCore.QRect(340, 470, 61, 21))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.label_10.setFont(font)
        self.label_10.setObjectName("label_10")
        self.image_root_index_2 = QtWidgets.QSpinBox(self.centralwidget)
        self.image_root_index_2.setGeometry(QtCore.QRect(200, 460, 71, 41))
        self.image_root_index_2.setMinimum(1)
        self.image_root_index_2.setMaximum(2)
        self.image_root_index_2.setProperty("value", 1)
        self.image_root_index_2.setObjectName("image_root_index_2")
        self.image_file_2 = QtWidgets.QPushButton(self.centralwidget)
        self.image_file_2.setGeometry(QtCore.QRect(50, 410, 141, 41))
        self.image_file_2.setObjectName("image_file_2")
        self.label_12 = QtWidgets.QLabel(self.centralwidget)
        self.label_12.setGeometry(QtCore.QRect(340, 600, 61, 21))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.label_12.setFont(font)
        self.label_12.setObjectName("label_12")
        self.image_file_3 = QtWidgets.QPushButton(self.centralwidget)
        self.image_file_3.setGeometry(QtCore.QRect(50, 540, 141, 41))
        self.image_file_3.setObjectName("image_file_3")
        self.label_14 = QtWidgets.QLabel(self.centralwidget)
        self.label_14.setGeometry(QtCore.QRect(80, 600, 91, 21))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.label_14.setFont(font)
        self.label_14.setObjectName("label_14")
        self.image_target_3 = QtWidgets.QComboBox(self.centralwidget)
        self.image_target_3.setGeometry(QtCore.QRect(410, 590, 141, 41))
        self.image_target_3.setObjectName("image_target_3")
        self.image_target_3.addItem("")
        self.image_target_3.addItem("")
        self.image_target_3.addItem("")
        self.image_root_index_3 = QtWidgets.QSpinBox(self.centralwidget)
        self.image_root_index_3.setGeometry(QtCore.QRect(200, 590, 71, 41))
        self.image_root_index_3.setMinimum(1)
        self.image_root_index_3.setMaximum(2)
        self.image_root_index_3.setProperty("value", 1)
        self.image_root_index_3.setObjectName("image_root_index_3")
        self.image_path_3 = QtWidgets.QLabel(self.centralwidget)
        self.image_path_3.setGeometry(QtCore.QRect(200, 540, 351, 41))
        font = QtGui.QFont()
        font.setPointSize(8)
        self.image_path_3.setFont(font)
        self.image_path_3.setText("")
        self.image_path_3.setObjectName("image_path_3")
        self.label_2 = QtWidgets.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(540, 130, 81, 21))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.label_2.setFont(font)
        self.label_2.setObjectName("label_2")
        self.n_images = QtWidgets.QSpinBox(self.centralwidget)
        self.n_images.setGeometry(QtCore.QRect(630, 120, 71, 41))
        self.n_images.setMinimum(1)
        self.n_images.setMaximum(3)
        self.n_images.setProperty("value", 3)
        self.n_images.setObjectName("n_images")
        self.upload = QtWidgets.QPushButton(self.centralwidget)
        self.upload.setGeometry(QtCore.QRect(680, 420, 171, 41))
        self.upload.setObjectName("upload")
        self.error_msg = QtWidgets.QLabel(self.centralwidget)
        self.error_msg.setGeometry(QtCore.QRect(20, 680, 1021, 61))
        font = QtGui.QFont()
        font.setPointSize(11)
        self.error_msg.setFont(font)
        self.error_msg.setText("")
        self.error_msg.setAlignment(QtCore.Qt.AlignCenter)
        self.error_msg.setObjectName("error_msg")
        self.label_15 = QtWidgets.QLabel(self.centralwidget)
        self.label_15.setGeometry(QtCore.QRect(50, 60, 171, 41))
        font = QtGui.QFont()
        font.setPointSize(15)
        font.setBold(False)
        font.setWeight(50)
        self.label_15.setFont(font)
        self.label_15.setObjectName("label_15")
        self.label_16 = QtWidgets.QLabel(self.centralwidget)
        self.label_16.setGeometry(QtCore.QRect(50, 220, 171, 41))
        font = QtGui.QFont()
        font.setPointSize(15)
        font.setBold(False)
        font.setWeight(50)
        self.label_16.setFont(font)
        self.label_16.setObjectName("label_16")
        self.version = QtWidgets.QLineEdit(self.centralwidget)
        self.version.setGeometry(QtCore.QRect(190, 120, 81, 41))
        self.version.setObjectName("version")
        self.server_password = QtWidgets.QLineEdit(self.centralwidget)
        self.server_password.setGeometry(QtCore.QRect(830, 290, 181, 41))
        self.server_password.setFrame(True)
        self.server_password.setEchoMode(QtWidgets.QLineEdit.Password)
        self.server_password.setObjectName("server_password")
        self.label_7 = QtWidgets.QLabel(self.centralwidget)
        self.label_7.setGeometry(QtCore.QRect(680, 290, 131, 41))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.label_7.setFont(font)
        self.label_7.setObjectName("label_7")
        self.sign = QtWidgets.QPushButton(self.centralwidget)
        self.sign.setGeometry(QtCore.QRect(680, 360, 171, 41))
        self.sign.setObjectName("sign")
        self.label_17 = QtWidgets.QLabel(self.centralwidget)
        self.label_17.setGeometry(QtCore.QRect(680, 220, 291, 41))
        font = QtGui.QFont()
        font.setPointSize(15)
        font.setBold(False)
        font.setWeight(50)
        self.label_17.setFont(font)
        self.label_17.setObjectName("label_17")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1059, 29))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)


        #############################################################
        #############################################################
        # Signal Connections
        #############################################################
        #############################################################
        self.n_images.valueChanged.connect(lambda:show_image_info(self, self.n_images.value()))
        self.image_file_1.clicked.connect(lambda:load_image_bin(self, image_number=1))
        self.image_file_2.clicked.connect(lambda:load_image_bin(self, image_number=2))
        self.image_file_3.clicked.connect(lambda:load_image_bin(self, image_number=3))
        self.sign.clicked.connect(lambda:sign_process(self))
        self.upload.clicked.connect(lambda:upload_process(self))

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "OTA-Application"))
        self.header.setText(_translate("MainWindow", "OEM Client Application"))
        self.label.setText(_translate("MainWindow", "Package version:"))
        self.urgency.setText(_translate("MainWindow", "Urgent package"))
        self.image_file_1.setText(_translate("MainWindow", "Upload image 1"))
        self.label_6.setText(_translate("MainWindow", "Root index"))
        self.image_target_1.setItemText(0, _translate("MainWindow", "Master ECU"))
        self.image_target_1.setItemText(1, _translate("MainWindow", "Target 1"))
        self.image_target_1.setItemText(2, _translate("MainWindow", "Target 2"))
        self.label_8.setText(_translate("MainWindow", "Target"))
        self.label_9.setText(_translate("MainWindow", "Root index"))
        self.image_target_2.setItemText(0, _translate("MainWindow", "Master ECU"))
        self.image_target_2.setItemText(1, _translate("MainWindow", "Target 1"))
        self.image_target_2.setItemText(2, _translate("MainWindow", "Target 2"))
        self.label_10.setText(_translate("MainWindow", "Target"))
        self.image_file_2.setText(_translate("MainWindow", "Upload image 2"))
        self.label_12.setText(_translate("MainWindow", "Target"))
        self.image_file_3.setText(_translate("MainWindow", "Upload image 3"))
        self.label_14.setText(_translate("MainWindow", "Root index"))
        self.image_target_3.setItemText(0, _translate("MainWindow", "Master ECU"))
        self.image_target_3.setItemText(1, _translate("MainWindow", "Target 1"))
        self.image_target_3.setItemText(2, _translate("MainWindow", "Target 2"))
        self.label_2.setText(_translate("MainWindow", "#Images:"))
        self.upload.setText(_translate("MainWindow", "Upload to Firebase"))
        self.label_15.setText(_translate("MainWindow", "Package info"))
        self.label_16.setText(_translate("MainWindow", "Images info"))
        self.version.setInputMask(_translate("MainWindow", "0.0.0"))
        self.label_7.setText(_translate("MainWindow", "Server Password:"))
        self.sign.setText(_translate("MainWindow", "Signing Request"))
        self.label_17.setText(_translate("MainWindow", "Server connections"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())