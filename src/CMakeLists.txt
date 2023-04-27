

file( GLOB SRC_FILES CONFIGURE_DEPENDS "*.cpp" )
file( GLOB HEADER_FILES CONFIGURE_DEPENDS "*.h" )

message( STATUS "[binaryReader/src/] sources : " ${SRC_FILES} )
message( STATUS "[binaryReader/src/] headers : " ${HEADER_FILES} )

add_library( binaryReader SHARED ${SRC_FILES} ${HEADER_FILES} )
