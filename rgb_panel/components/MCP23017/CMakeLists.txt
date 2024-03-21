# set(COMPONENT_SRCS "MCP23017.cpp")
# idf_component_register(
#     SRCS "MCP23017.cpp"
#     INCLUDE_DIRS "."
#     PRIV_REQUIRES arduino-esp32  # Ajoutez cette ligne
#     NAME MCP23017  # Renommez la cible en MCP23017
# )
# register_component()
set(COMPONENT_SRCS "MCP23017.cpp")
set(COMPONENT_ADD_INCLUDEDIRS ".")
set(COMPONENT_PRIV_REQUIRES arduino-esp32)
register_component()