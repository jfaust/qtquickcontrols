import Qt 4.7

Item {
    id: adjoiningVisual
    property int adjoins: 0    // use enum Qt::DockWidgetArea? see http://doc.trolltech.com/4.7/qt.html#DockWidgetArea-enum
    property Item styledItem
    property Component styling

//    Qt::LeftDockWidgetArea	0x1
//    Qt::RightDockWidgetArea	0x2
//    Qt::TopDockWidgetArea	0x4
//    Qt::BottomDockWidgetArea	0x8

    Item {
        anchors.fill: parent

        AdjoiningCorner { corner: "TopLeftCorner"
            adjoining: adjoins&0x01 || adjoins&0x04; styledItem: adjoiningVisual.styledItem }
        AdjoiningCorner { corner: "TopRightCorner"
            adjoining: adjoins&0x02 || adjoins&0x04; styledItem: adjoiningVisual.styledItem }
        AdjoiningCorner { corner: "BottomLeftCorner"
            adjoining: adjoins&0x01 || adjoins&0x08; styledItem: adjoiningVisual.styledItem }
        AdjoiningCorner { corner: "BottomRightCorner"
            adjoining: adjoins&0x02 || adjoins&0x08; styledItem: adjoiningVisual.styledItem }
    }
}