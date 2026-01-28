
add_subdirectory(nlohmann_json)

configure_file(${CMAKE_SOURCE_DIR}/conf/config.json ${CMAKE_CURRENT_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/conf/requests.json ${CMAKE_CURRENT_BINARY_DIR})


add_executable(search_engine
    main.cpp
    ConverterJSON.cpp
    InvertedIndex.cpp
    SearchServer.cpp
)
add_custom_command(TARGET search_engine POST_BUILD
    # Конфиги
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/conf/config.json
        $<TARGET_FILE_DIR:search_engine>/config.json
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/conf/requests.json
        $<TARGET_FILE_DIR:search_engine>/requests.json

    # Файлы данных - прямо в build/, не в подпапке
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/file001.txt
        $<TARGET_FILE_DIR:search_engine>/file001.txt
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/file002.txt
        $<TARGET_FILE_DIR:search_engine>/file002.txt
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/file003.txt
        $<TARGET_FILE_DIR:search_engine>/file003.txt

    COMMENT "Copying files to executable directory"
)
target_include_directories(search_engine PUBLIC  ${INCLUDE_DIR})


target_link_libraries(search_engine PRIVATE nlohmann_json)
