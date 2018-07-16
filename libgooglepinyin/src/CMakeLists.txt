set(LIBGOOGLEPINYIN_API_VERSION "0.1.0")

set(LIBGOOGLEPINYIN_ABI_VERSION "0.1.0")
set(LIBGOOGLEPINYIN_ABI_MAJOR_VERSION "0")

set(GOOGLEPINYIN_SRC
	dictbuilder.cpp
	dictlist.cpp
	dicttrie.cpp
	lpicache.cpp
	matrixsearch.cpp
	mystdlib.cpp
	ngram.cpp
	pinyinime.cpp
	searchutility.cpp
	spellingtable.cpp
	spellingtrie.cpp
	splparser.cpp
	sync.cpp
	userdict.cpp
	utf16char.cpp
	utf16reader.cpp
)

add_library(googlepinyin SHARED ${GOOGLEPINYIN_SRC})
target_link_libraries(googlepinyin pthread )
set_target_properties(googlepinyin PROPERTIES
    VERSION ${LIBGOOGLEPINYIN_ABI_VERSION}
    SOVERSION ${LIBGOOGLEPINYIN_ABI_MAJOR_VERSION}
    LINK_FLAGS ${CMAKE_SHARED_LINKER_FLAGS}
)

if (ENABLE_STATIC)

add_library(googlepinyin-static STATIC ${GOOGLEPINYIN_SRC})
SET_TARGET_PROPERTIES(googlepinyin-static PROPERTIES
    OUTPUT_NAME "googlepinyin"
    PREFIX "lib"
)

install(TARGETS googlepinyin-static
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
endif (ENABLE_STATIC)
install(TARGETS googlepinyin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
