var jsdom = require("jsdom")
  , nu = require('nodeutil')

jsdom.env(
//"http://recipe.ytower.com.tw/Channel/Practice/%E9%9B%BB%E9%8D%8B/2",
  'http://ent.qq.com/a/20140209/000023.htm',
  ["http://code.jquery.com/jquery.js"],
  /**
   * 這邊是想要透過jQuery取出頁面上所有鏈結的位置
   */
  function (errors, window) {
    window.$("a").each(function(i, a){
      console.log(a.href)
    })
    window.$("img").each(function(i, img){
      console.log(img.src)
    })
  }
);

