HTTP/1.1 200 OK
Date: Thu, 12 Dec 2019 17:38:37 GMT
Content-Type: text/javascript; charset=UTF-8
Content-Length: 362
Connection: keep-alive
Server: nginx
Expires: Sun, 23 Sep 2012 03:52:54 GMT
Cache-Control: private, no-cache, no-cache=Set-Cookie, proxy-revalidate
Pragma: no-cache

(function(){var f="widget.js";if(!('__proto__' in {}))f="widget.ie.js";var d=document;var h=d.createElement("script");h.type="text/javascript";h.async=true;h.src=("https:"==d.location.protocol?"https":"http")+"://static.hypercomments.com/widget/hcembed/19454/ru/274/1/"+f;var s=d.getElementsByTagName("script")[0];s.parentNode.insertBefore(h,s.nextSibling);})();