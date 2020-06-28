# node自定义C/C++插件(addon)

# 必要条件
    node-gyp 

# 构建步骤
    1. 定义c/c++文件暴露接口方法
    2. 创建binding.gyp
    3. node-gyp configure 
    4. node-gyp build
    5. js文件中引入插件调用方法(只需要引用生成的xx.node文件其他文件可不要
    6. 重新clean并且构建: node-gyp rebuild
    