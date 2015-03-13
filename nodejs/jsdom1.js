var jsdom = require("jsdom")

var doc = jsdom.jsdom('<script> document.write(\'<a href="http://abc.com/">anchor</a>\');</script>');
console.log(doc.innerHTML);
doc = jsdom.jsdom('http://ent.qq.com/a/20140209/000517.htm');
console.log(doc.innerHTML);

