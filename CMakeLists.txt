cmake_minimum_required(VERSION 3.5.0)
#设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


#vcpkg安装路径
SET(CMAKE_TOOLCHAIN_FILE "G:/vcpkg/scripts/buildsystems/vcpkg.cmake")

#设置Debug版本
set(CMAKE_BUILD_TYPE Debug)

project(protobuf-demo VERSION 0.1.0 LANGUAGES C CXX)


find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    message(STATUS "Found CCache: ${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()



file(GLOB SOURCE_FILES 
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
  )


  find_package(Protobuf CONFIG REQUIRED)
  SET(PROTOBUF_PROTOC_EXECUTABLE "G:/vcpkg/packages/protobuf_x64-windows/tools/protobuf/protoc.exe")
  file(GLOB PROTOBUF_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto)
  protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOBUF_SOURCE_FILES})
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/build ${CMAKE_CURRENT_SOURCE_DIR}/include)

  find_package(Boost)
  if (Boost_FOUND)
      # 设置 Boost 库的使用方式
      set(Boost_USE_STATIC_LIBS ON)
      set(Boost_USE_STATIC_RUNTIME ON)
      # 设置 Boost 的包含目录
      if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
          set(Boost_INCLUDE_DIR ${_VCPKG_INSTALLED_DIR}/x64-windows/include)
      endif()
      if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
          set(Boost_INCLUDE_DIR ${_VCPKG_INSTALLED_DIR}/x64-linux/include)
      endif()
  endif()
  
  foreach(SOURCE_FILE ${SOURCE_FILES})
      # 提取文件名（不带路径和扩展名）
      get_filename_component(TARGET_NAME ${SOURCE_FILE} NAME_WE)
      # 生成可执行文件
      add_executable(${TARGET_NAME} ${SOURCE_FILE} ${PROTO_SRCS} ${PROTO_HDRS})
      # 链接 Protobuf 库
      target_link_libraries(${TARGET_NAME} PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
      if (Boost_FOUND)
          # 设置 Boost 的包含目录和库目录
          target_include_directories(${TARGET_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
          link_directories(${Boost_LIBRARY_DIRS})
          # 链接 Boost 库
          target_link_libraries(${TARGET_NAME} PRIVATE ${Boost_LIBRARIES})
      endif ()
      # 添加测试
      add_test(NAME ${TARGET_NAME}_Test COMMAND ${TARGET_NAME})
  endforeach()


# 添加手动生成protobuf目标
add_custom_target(gen-protobuf 
COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} 
--cpp_out=${CMAKE_BINARY_DIR}
--proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto ${PROTOBUF_SOURCE_FILES}
COMMENT "Generating Protobuf CPP files..."
)


# 添加清理目标
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_BINARY_DIR}/clean-all.cmake"
    COMMENT "Cleanup completed..."
)

# 生成clean-all.cmake文件
#cmake --build build --target clean-all
file(WRITE "${CMAKE_BINARY_DIR}/clean-all.cmake" "

#if (EXISTS \"${CMAKE_BINARY_DIR}/CMakeCache.txt\")
#    file(REMOVE \"${CMAKE_BINARY_DIR}/CMakeCache.txt\")
#endif()
if (EXISTS \"${CMAKE_BINARY_DIR}/CMakeFiles\")
    file(REMOVE_RECURSE \"${CMAKE_BINARY_DIR}/CMakeFiles\")
endif()
if (EXISTS \"${CMAKE_BINARY_DIR}/Testing\")
    file(REMOVE_RECURSE \"${CMAKE_BINARY_DIR}/Testing\")
endif()



file(REMOVE_RECURSE  ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

file(GLOB SOURCE_FILES \"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp\")
foreach(SOURCE_FILE \${SOURCE_FILES})
  get_filename_component(TARGET_NAME \${SOURCE_FILE} NAME_WE)
  file(REMOVE_RECURSE \${CMAKE_BINARY_DIR}/\${TARGET_NAME}.dir)
endforeach()

")


include(CTest)
enable_testing()