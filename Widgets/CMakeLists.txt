message (STATUS "Configurating Widgets")

target_sources (${TARGET_NAME}
    PRIVATE
        LinkLine.cpp LinkLine.hh
        DatabridgePoint.cpp DatabridgePoint.hh
        DatabridgeWidget.cpp DatabridgeWidget.hh
        DatabridgeBlock.cpp DatabridgeBlock.hh
        DatabridgeNode.cpp DatabridgeNode.hh
        DatabridgeNodeSlot.cpp DatabridgeNodeSlot.hh
        DatabridgeLink.cpp DatabridgeLink.hh
        DatabridgeOrthogonalLink.cpp DatabridgeOrthogonalLink.hh)
