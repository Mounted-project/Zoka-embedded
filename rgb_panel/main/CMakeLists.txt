# # file(GLOB_RECURSE SRC_UI ${CMAKE_SOURCE_DIR} "ui/*.c")
# idf_component_register(SRCS "main.cpp"
#                        INCLUDE_DIRS "." "ui")
# set(CMAKE_CXX_STANDARD 23)
# add_compile_options(-ffunction-sections -fdata-sections -Wl,--gc-sections)


#Add sources from ui directory
file(GLOB_RECURSE SRC_UI ${CMAKE_SOURCE_DIR} "ui/*.c")

idf_component_register(SRCS "main.cpp" ${SRC_UI}
                    INCLUDE_DIRS "." "ui")