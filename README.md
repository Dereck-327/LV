# 要求

- google cxx 代码规范  
- 分模块写cmakelists，每个模块单独可通过单元测试
- 需要写每个模块的单元测试 用googletest  
- 第三方库用`FetchContent_Declare`拉取到third_party

```cmd
rmdir /s /q build
cmake -S . -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build build --target LV_app --verbose
cmake --build build --target LV_tests --verbose

```
