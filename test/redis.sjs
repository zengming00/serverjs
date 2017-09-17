var r = new Redis('127.0.0.1', 6379);

var k = '%s:$^&*';

print(r.cmd('set %s %s', k, 'helloworld'));
print(r.cmd('get %s', k));
print(r.cmd('keys *'));

print(r.cmd('DBSIZE'));

var bstr = '12\u0003\u000456';
print('bstr.length='+bstr.length);
if(bstr.length === 6){
	print(r.cmd('set %b %b', bstr, bstr.length, bstr, bstr.length));
	print(r.cmd('get %b', bstr, bstr.length));
}

print('---------------');
print(r.cmd('keys *'));

r.close();
print('done.');
