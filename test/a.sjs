var b  = require("./b.sjs");

function foo(){
	print("a.foo()");
}

foo();
print(typeof b);
b.foo();


var obj = {
  a: true,
  b: 123,
  c: 3.1415,
  d: "helloworld",
  e: /^$/g,
  f: [11,22],
};

print(JSON.stringify(obj, null, 2));

throw new Error('test error');
