test-toys, 编码练习
=========

1.cpp, c/c++方面的测试程序，其中的libmisc.a是基础功能，为其它目录所依赖
  - 日志宏，日志中自动添加函数、文件名、行号、进程号等信息，方便调试。包括WARNING/NOTICE两个级别。
    - NOTICE 2014-02-05 00:02:02 (t_lua.cpp:85,test_conf:3412) width=3, height=14
  - 时间函数，纯粹测试使用
  - leveldb中的testharness摘录出来并试用了一下

2.lua, lua相关的学习测试
  - 依赖：lua,expat
  - 使用so之前，需要先设置LD_LIBRARY_PATH,source env.sh即可  
  - t_lua.cpp,综合多种功能的一个入口程序
    - conf t_conf.lua, 测试使用lua给c/cpp作为配置文件的功能
    - stack, 打印栈的功能的试用
  - luadir.so, dir封装为lua函数。测试命令:lua dir.lua
  - luaexpat.so, xml解析库封装为lua函数。测试命令：lua expat.lua


3. py, python学习过程中的一些练习
