cmake_minimum_required(VERSION 3.10)
project(Pentagram LANGUAGES CXX C DESCRIPTION "A framwork for building apps" HOMEPAGE_URL "https://github.com/ShoweryCellar34/Pentagram")

set(GLFW_LIBRARY_TYPE "STATIC" CACHE STRING "Library type override for GLFW (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")
add_subdirectory(vendors/glfw)

add_subdirectory(vendors/glm)

add_subdirectory(vendors/spdlog)

add_subdirectory(vendors/stb)

set(RELATIVE_PATH "vendors/glfw/include")
set(IMGUI_GLFW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/${RELATIVE_PATH}")
add_subdirectory(vendors/imgui)

add_subdirectory(vendors/glad)

add_library(Pentagram STATIC source/event.cpp source/file.cpp source/gl.cpp source/image.cpp source/init.cpp source/window.cpp)
set_target_properties(Pentagram PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(Pentagram spdlog::spdlog glm::glm glfw imgui glad stb)
target_include_directories(Pentagram PUBLIC headers)


function(combine_archives output_archive list_of_input_archives)
    set(mri_file ${TEMP_DIR}/${output_archive}.mri)
    set(FULL_OUTPUT_PATH ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/lib${output_archive}.a)
    file(WRITE ${mri_file} "create ${FULL_OUTPUT_PATH}\n")
    FOREACH(in_archive ${list_of_input_archives})
        file(APPEND ${mri_file} "addlib ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/lib${in_archive}.a\n")
    ENDFOREACH()
    file(APPEND ${mri_file} "save\n")
    file(APPEND ${mri_file} "end\n")

    set(output_archive_dummy_file ${TEMP_DIR}/${output_archive}.dummy.cpp)
    add_custom_command(OUTPUT ${output_archive_dummy_file}
                       COMMAND touch ${output_archive_dummy_file}
                       DEPENDS ${list_of_input_archives})

    add_library(${output_archive} STATIC ${output_archive_dummy_file})
    add_custom_command(TARGET ${output_archive}
                       POST_BUILD
                       COMMAND ar -M < ${mri_file})
endfunction(combine_archives)
