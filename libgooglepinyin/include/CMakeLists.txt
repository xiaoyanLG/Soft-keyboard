set(GOOGLEPINYIN_HEADERS
atomdictbase.h
dictbuilder.h
dictdef.h
dictlist.h
dicttrie.h
lpicache.h
matrixsearch.h
mystdlib.h
ngram.h
pinyinime.h
searchutility.h
spellingtable.h
spellingtrie.h
splparser.h
sync.h
userdict.h
utf16char.h
utf16reader.h
)

install(FILES ${GOOGLEPINYIN_HEADERS} DESTINATION ${includedir})
