var r = new Redis('127.0.0.1', 6379);
print(r.cmd('set sjs:redis helloworld'));
print(r.cmd('get sjs:redis'));

r.close();
