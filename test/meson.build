
test_headers = include_directories('../include')

ptr_test = executable('ptr_test'
    , 'ptr_main.cpp'
    , dependencies: [genericimg_deps]
    , include_directories : test_headers
    , link_with : project_target)
test('ptr_test', ptr_test)

buf_test = executable('buf_test'
    , 'buf_main.cpp'
    , dependencies: [genericimg_deps]
    , include_directories : test_headers 
    , link_with : project_target)
test('buf_test', buf_test)

