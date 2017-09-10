// this is b.sjs

function foo(){
	print("b.foo()")
}


var f = new File('./b.sjs');
var n = f.readByte();
print("=====>", n.toString(16));
print("=====>", f.readLine());
f.close();

print("call gc()");
gc();

exports.foo = foo;
