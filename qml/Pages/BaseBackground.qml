import QtQuick 2.15
import Style 1.0

Rectangle {
    anchors.fill: parent
    gradient: Gradient {
        GradientStop {
            position: 0.00
            color: Style.pagesBack.pageBackColorS
        }

        GradientStop {
            position: 1.00
            color: Style.pagesBack.pageBackColorE
        }
    }
}
