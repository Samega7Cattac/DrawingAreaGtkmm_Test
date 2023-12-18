message (STATUS "Configurating Graph")

target_sources (${TARGET_NAME}
    PRIVATE
        Graph.cpp Graph.hh
        GraphAdjacencyMatrix.cpp GraphAdjacencyMatrix.hh
        GraphNode.cpp GraphNode.hh
        )
