cmake_minimum_required(VERSION 2.8.12)

project(iotdb_mongodb_odl_server)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include_directories(./src)


#seting file pathes
file(GLOB ServerHeaders ./src/server/*.h)
file(GLOB ServerCpp ./src/server/*.cpp)
file(GLOB ServerHpp ./src/server/*.hpp)

file(GLOB coreHeaders ./src/core/*.h)
file(GLOB coreCpp ./src/core/*.cpp)
file(GLOB coreHpp ./src/core/*.hpp)

file(GLOB SecurityHeaders ./src/database/security/*.h)
file(GLOB SecurityCpp ./src/database/security/*.cpp)
file(GLOB SecurityHpp ./src/database/security/*.hpp)

file(GLOB RequestHeaders ./src/controller/*.h)
file(GLOB RequestCpp ./src/controller/*.cpp)
file(GLOB RequestHpp ./src/controller/*.hpp)

#************************

file(GLOB DeleteCollectionHeaders   ./src/controller/collection/delete/*.h)
file(GLOB DeleteCollectionCpp       ./src/controller/collection/delete/*.cpp)
file(GLOB DeleteCollectionHpp       ./src/controller/collection/delete/*.hpp)


file(GLOB FindCollectionHeaders     ./src/controller/collection/find/*.h)
file(GLOB FindCollectionCpp         ./src/controller/collection/find/*.cpp)
file(GLOB FindCollectionHpp         ./src/controller/collection/find/*.hpp)


file(GLOB GetInfoCollectionHeaders  ./src/controller/collection/get_info/*.h)
file(GLOB GetInfoCollectionCpp      ./src/controller/collection/get_info/*.cpp)
file(GLOB GetInfoCollectionHpp      ./src/controller/collection/get_info/*.hpp)


file(GLOB InsertCollectionHeaders   ./src/controller/collection/insert/*.h)
file(GLOB InsertCollectionCpp       ./src/controller/collection/insert/*.cpp)
file(GLOB InsertCollectionHpp       ./src/controller/collection/insert/*.hpp)


file(GLOB UpdateCollectionHeaders   ./src/controller/collection/update/*.h)
file(GLOB UpdateCollectionCpp       ./src/controller/collection/update/*.cpp)
file(GLOB UpdateCollectionHpp       ./src/controller/collection/update/*.hpp)

file(GLOB UserControllersHeaders   ./src/controller/user/*.h)
file(GLOB UserControllersCpp       ./src/controller/user/*.cpp)


file(GLOB DatabaseHeaders  ./src/database/*.h)
file(GLOB DatabaseCpp      ./src/database/*.cpp)
file(GLOB DatabaseHpp      ./src/database/*.hpp)


#library
add_library(Server
  ${ServerHeaders}
  ${ServerCpp}
  ${ServerHpp}
  )

add_library(Security
  ${SecurityHeaders}
  ${SecurityCpp}
  ${SecurityHpp}
  )

add_library(core
  ${coreHeaders}
  ${coreCpp}
  ${coreHpp}
  )


add_library(Request

  ${RequestHeaders}
  ${RequestCpp}
  ${RequestHpp}

  ${DeleteCollectionHeaders}
  ${DeleteCollectionCpp}
  ${DeleteCollectionHpp}


  ${FindCollectionHeaders}
  ${FindCollectionCpp}
  ${FindCollectionHpp}

  ${GetInfoCollectionHeaders}
  ${GetInfoCollectionCpp}
  ${GetInfoCollectionHpp}

  ${InsertCollectionHeaders}
  ${InsertCollectionCpp}
  ${InsertCollectionHpp}

  ${UpdateCollectionHeaders}
  ${UpdateCollectionCpp}
  ${UpdateCollectionHpp}


  ${UserControllersHeaders}
  ${UserControllersCpp}
  )

target_link_libraries(Request
  Database
  Security
  core
  )

target_link_libraries(Security
  Database
  )

add_library(Database
  ${DatabaseHeaders}
  ${DatabaseCpp}
  ${DatabaseHpp}
  )


set (CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++17 -D__STRICT_ANSI__")

find_package (Threads)
find_package(libbsoncxx)
find_package(libmongocxx)

include_directories(${LIBBSONCXX_INCLUDE_DIRS} ${LIBMONGOCXX_INCLUDE_DIRS} )


find_package(Boost 1.58 REQUIRED COMPONENTS
  date_time
  filesystem
  iostreams
  thread
  )

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(${PROJECT_NAME} "posix_main.cpp" )
  target_link_libraries(${PROJECT_NAME}
    Boost::date_time
    Boost::filesystem
    Boost::iostreams
    Boost::thread

    ${CMAKE_THREAD_LIBS_INIT}
    ${LIBBSONCXX_LIBRARIES}
    ${LIBMONGOCXX_LIBRARIES}

    Server
    Request
    Database
    Security
    )

  target_link_libraries(Database
    Boost::date_time
    Boost::iostreams
    ${LIBBSONCXX_LIBRARIES}
    ${LIBMONGOCXX_LIBRARIES}
    )
endif(Boost_FOUND)

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

