import QtQuick
import QtQuick.Controls
import Qt.labs.platform as Platform
import Vimu.Qml.Ctrls

ApplicationWindow {
    id: mianwindow
    width: 1000
    height: 600
    visible: true
    title: qsTr("VmMso")

    function usbDevCallBack(add_remove)
    {
        console.log("QML get message:",add_remove);
    }

    Action {
        id: openCh1Action
        //shortcut: StandardKey.Open
        onTriggered:{
            vmusbwave.qmlcallcpp_style1();   //
            vmusbwave.qmlcallcpp_style2(1);

            if(vmusbwave.isStar())
                vmusbwave.stop(1);
            else
                vmusbwave.star(1);
        }
    }

    Platform.MenuBar {
        Platform.Menu {
            title: qsTr("&File")

            Platform.MenuItem {
                text: qsTr("&Open")
                onTriggered: openDialog.open()
            }
            Platform.MenuItem {
                text: qsTr("&Save As...")
                onTriggered: saveDialog.open()
            }
            Platform.MenuItem {
                text: qsTr("&Quit")
                onTriggered: close()
            }
        }
    }

    ToolBar {
        id: toolbar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        //anchors.margins: 4
        background: Rectangle {
                    color: "#333333"
                }

        Column {
            padding: 4
            spacing: 4
            width: parent.width

            GroupBox{
                width: triggerCtrl.width
                padding: 4
                spacing: 4

                Column {
                    Row {
                        anchors.horizontalCenter: parent.horizontalCenter
                        ToolButton {
                             id: resetDllButton
                             text: "Reset Dll"
                             focusPolicy: Qt.TabFocus
                             //enabled: textArea.selectedText

                             onClicked:{
                                 vmusbwave.resetDll();
                             }
                       }
                        ToolButton {
                             id: resetDevButton
                             text: "Reset Device"
                             focusPolicy: Qt.TabFocus
                             //enabled: textArea.selectedText

                             onClicked:{
                                 vmusbwave.resetDevice();
                             }
                       }
                    }

                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: sampleCombox.verticalCenter
                            text: "Sample"
                        }

                        ComboBox {
                            id: sampleCombox

                            onActivated: {
                                console.log("sampleCombox " + currentText);
                                vmusbwave.setsample(Number(currentText));
                            }
                        }
                    }

                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: captureLengthText.verticalCenter
                            text: "Length(KB)"
                        }

                        TextField {
                            id: captureLengthText
                            validator: IntValidator {
                                            id: captureLengthTextIntValidator
                                            bottom: 0;
                                            top: 1000;
                            }

                            onEditingFinished: vmusbwave.setCaptureLength(Number(text));
                        }
                    }

                    Row {
                        anchors.horizontalCenter: parent.horizontalCenter
                        CheckBox {
                            id: captureEn
                            text: "Capture"
                            focusPolicy: Qt.TabFocus
                            //enabled: textArea.selectedText
                            onClicked: vmusbwave.captureEnable(captureEn.checked);
                        }
                    }
                }
            }

            GroupBox {
                padding: 4
                spacing: 4
                width: triggerCtrl.width
                anchors.horizontalCenter: parent.horizontalCenter

                Column {
                    Label{
                        text: "AC/DC"
                    }

                    Column {
                        CheckBox {
                            id: acCh1
                            text: "AC(CH1)"
                            focusPolicy: Qt.TabFocus
                            onClicked: vmusbwave.setAcDcCh1(acCh1.checked);
                        }

                        CheckBox {
                            id: acCh2
                            text: "AC(CH2)"
                            focusPolicy: Qt.TabFocus
                            //enabled: textArea.selectedText
                            onClicked: vmusbwave.setAcDcCh2(acCh2.checked);
                        }
                    }
                }
            }

            GroupBox {
                id: triggerCtrl
                padding: 4
                spacing: 4
                //width: sampleCtrl.width
                anchors.horizontalCenter: parent.horizontalCenter

                Column {
                    padding: 4
                    spacing: 4
                    Label{
                        text: "Trigger"
                    }

                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: triggerModeCombox.verticalCenter
                            text: "Trigger Mode"
                        }

                        ComboBox {
                            id: triggerModeCombox
                            model:["Auto","Normal"]

                            onActivated: {
                                console.log("triggerModeCombox " + currentText);
                                vmusbwave.setTriggerMode(triggerModeCombox.currentIndex);
                            }
                        }
                    }
                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: triggerStyleCombox.verticalCenter
                            text: "Trigger Style"
                        }

                        ComboBox {
                            id: triggerStyleCombox
                            model:["not trigger","Rising edge","Falling edge","Edge",
                                "Positive Pulse width(>)","Positive Pulse width(>)","Positive Pulse width(<>)",
                                "Negative Pulse width(>)","Negative Pulse width(>)","Negative Pulse width(<>)"]

                            onActivated: {
                                console.log("triggerStyleCombox " + currentText);
                                vmusbwave.setTriggerStyle(triggerStyleCombox.currentIndex);
                            }
                        }
                    }

                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: triggerSourceCombox.verticalCenter
                            text: "Trigger Source"
                        }

                        ComboBox {
                            id: triggerSourceCombox
                            model:["CH1","CH2"]

                            onActivated: {
                                console.log("triggerSourceCombox " + currentText);
                                vmusbwave.setTriggerSource(triggerSourceCombox.currentIndex);
                            }
                        }
                    }

                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: triggerLevelEdit.verticalCenter
                            text: "Trigger Level(mV)"
                        }

                        TextField {
                            id: triggerLevelEdit
                            validator: IntValidator {
                                            id: triggerLevelEditIntValidator
                                            bottom: -5000;
                                            top: 5000;
                            }

                            onEditingFinished: vmusbwave.setTriggerLevel(Number(text));
                        }
                    }
                }
            }
        }
    }

    ToolBar {
        id: toolbarright
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        //anchors.bottom: childrenRect.bottom
        anchors.margins: 4

        background: Rectangle {
                    color: "#333333"
                }

        Column {
            spacing: 4

            GroupBox {
                Column  {
                    Label{
                        text: "CH1"
                    }

                    Dial {
                        id: dial_ch1
                        inputMode: Dial.Circular
                        from: 24
                        to: 31
                        stepSize: 1
                        value: 30
                        anchors.horizontalCenter: parent.horizontalCenter

                        property double interal: 0;
                        onMoved: {
                            interal = vmdsoplot.getAxisInterval(value);
                            vmdsoplot.setYAxisRange("CH1", -6*interal, interal);
                            vmdsoplot.redraw();
                        }
                    }
                }
            }

            GroupBox {
                Column  {
                    Label{
                        text: "CH2"
                    }

                    Dial {
                        id: dial_ch2
                        inputMode: Dial.Circular
                        from: 24
                        to: 31
                        stepSize: 1
                        value: 30
                        anchors.horizontalCenter: parent.horizontalCenter

                        property double interal: 0;
                        onMoved: {
                            interal = vmdsoplot.getAxisInterval(value);
                            vmdsoplot.setYAxisRange("CH2", -4*interal, interal);
                            vmdsoplot.redraw();
                        }
                    }
                }
            }

            GroupBox {
                Column  {
                    Label{
                        text: "Time"
                    }

                    Dial {
                        id: dial_time
                        inputMode: Dial.Circular
                        from: 36
                        to: 57
                        stepSize: 1
                        value: 51
                        anchors.horizontalCenter: parent.horizontalCenter

                        property double interal: 0;
                        onMoved: {
                            interal = vmdsoplot.getAxisInterval(value);
                            //console.log("interal " + -4*interal + " " + interal);
                            vmdsoplot.setTimeAxisRange(-5*interal, interal);
                            vmdsoplot.redraw();
                        }
                    }
                }
            }
        }
    }

    VmSimplePlot {
        id: vmdsoplot
        objectName: "objectvmdsoplot"
        anchors.left: toolbar.right
        anchors.top:  parent.top
        anchors.right: toolbarright.left
        anchors.bottom:  parent.bottom
        anchors.margins: 4

        property double interal: 0;
        Component.onCompleted: {
            vmdsoplot.addAxis("CH1", "#FFFF00");
            interal = vmdsoplot.getAxisInterval(dial_ch1.value);
            vmdsoplot.setYAxisRange("CH1", -6*interal, interal);

            vmdsoplot.addAxis("CH2", "#FF0000");
            interal = vmdsoplot.getAxisInterval(dial_ch2.value);
            vmdsoplot.setYAxisRange("CH2", -4*interal, interal);
            interal = vmdsoplot.getAxisInterval(dial_time.value);

            vmdsoplot.setTimeAxisRange(-5*interal, interal);

            vmdsoplot.computeAxisPos();
            vmdsoplot.redraw();
        }

        onYAxisRangeChanged: function (trackname, anArray) {
            if(trackname==="CH1")
                vmusbwave.setPlotRangeCh1(anArray);
            else
                vmusbwave.setPlotRangeCh2(anArray);
        }

        onUpdateDataSuccess: vmusbwave.nextCapture();
    }

    VmUsbWave {
        id: vmusbwave
        objectName: "objectvmusbwave"

        onDevStateChanged: function (devstate) {
            vmdsoplot.updatDevState(devstate);

            sampleCombox.model = vmusbwave.getsupportsamples();
            sampleCombox.currentIndex = sampleCombox.indexOfValue(vmusbwave.getsample().toString());
            captureLengthTextIntValidator.top = vmusbwave.getMaxCaptureLength();
            captureLengthText.text = vmusbwave.getCaptureLength().toString();

            acCh1.checked = vmusbwave.getAcDcCh1();
            acCh2.checked = vmusbwave.getAcDcCh2();

            triggerModeCombox.enabled = vmusbwave.isSupportHardTrigger();
            triggerStyleCombox.enabled = vmusbwave.isSupportHardTrigger();
            triggerSourceCombox.enabled = vmusbwave.isSupportHardTrigger();
            triggerLevelEdit.enabled = vmusbwave.isSupportHardTrigger();
            triggerModeCombox.currentIndex = vmusbwave.getTriggerMode();
            triggerStyleCombox.currentIndex = vmusbwave.getTriggerStyle();
            triggerSourceCombox.currentIndex = vmusbwave.getTriggerSource();
            triggerLevelEdit.text = vmusbwave.getTriggerLevel().toString();
            console.log("devStateChanged " + devState);
        }

        onUpdateDatas: function (real_length, sample) {
            //for (var i=0; i<anArray.length; i++)
            //    console.log("Array item:", anArray[i])
            vmdsoplot.updateDatas(real_length, sample);
            vmdsoplot.redraw();
        }
    }

}

