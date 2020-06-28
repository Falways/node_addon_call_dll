console.log("first line--")
// hello.js
const addon = require('./build/Release/hello');

// 打印: 'world'
console.log(addon.hello());

console.log(addon.add(15,11))

addon.callFuc(function (msg) {
    console.log(msg)
})

console.log(addon.getObj("body"))

addon.loadDll()
