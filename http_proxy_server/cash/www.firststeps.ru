HTTP/1.1 200 OK
Date: Thu, 12 Dec 2019 17:46:25 GMT
Server: Apache/2.4.25 (Unix) PHP/5.6.29
X-Powered-By: PHP/5.6.29
Content-Length: 4329
Keep-Alive: timeout=5, max=100
Connection: Keep-Alive
Content-Type: text/html; charset=cp1251

<html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1251">
<!-- memcache -->
<title>www.ПЕРВЫЕ ШАГИ.ru :: 1.27.1.21 Функция EnumWindowsProc</title><script async src="//pagead2.googlesyndication.com/pagead/js/adsbygoogle.js"></script><script>(adsbygoogle = window.adsbygoogle || []).push({google_ad_client: "ca-pub-2721325097469880",enable_page_level_ads: true});</script></head><link rel=stylesheet type=text/css href=/2.css><body text=black bgcolor=white link=blue vlink=blue alink=red><script async src="//pagead2.googlesyndication.com/pagead/js/adsbygoogle.js"></script>
<!-- page top adaptive -->
<ins class="adsbygoogle"
     style="display:block"
     data-ad-client="ca-pub-2721325097469880"
     data-ad-slot="2551301440"
     data-ad-format="auto"
     data-full-width-responsive="true"></ins>
<script>
(adsbygoogle = window.adsbygoogle || []).push({});
</script><h2>1.27.1.21 Функция EnumWindowsProc</h2><script type="text/javascript">//<![CDATA[
var _tmr = _tmr || [];
_tmr.push({id:"23099",type:"pageView",start:(new Date()).getTime()});
(function (d,w){
var ts=d.createElement("script");ts.type="text/javascript";ts.async=true;
ts.src=(d.location.protocol=="https:"?"https:":"http:")+"//top-fwz1.mail.ru/js/code.js";
var f=function(){var s=d.getElementsByTagName("script")[0];s.parentNode.insertBefore(ts,s);};
if (w.opera == "[object Opera]"){d.addEventListener("DOMContentLoaded",f,false);}else{f();}
})(document,window);
//]]></script><noscript><div style="position:absolute;left:-10000px;">
<img src="//top-fwz1.mail.ru/counter?id=23099;js=na" style="border:0;" height="1" width="1" alt="top@Mail.ru"/>
</div></noscript><div style="margin:10px;"><div style="margin-top:100px;margin-left:20px;float:right">
<script type="text/javascript">
    google_ad_client = "ca-pub-2721325097469880";
    google_ad_slot = "4810385248";
    google_ad_width = 120;
    google_ad_height = 600;
</script>
<!-- vertical right column -->
<script type="text/javascript"
src="//pagead2.googlesyndication.com/pagead/show_ads.js">
</script>
</div><p>(ПеречислениеОконныхПроцедур)
<p>Функция <b>EnumWindowsProc</b> - определяемая программой функция повторного вызова, которая принимает дескрипторы окна верхнего уровня в результате вызова функции <a href=r.php?67>EnumWindows</a> или <b>EnumDesktopWindows</b>.
<p>Синтаксис
<pre>
BOOL CALLBACK EnumWindowsProc
(
	HWND hwnd,	<i>// дескриптор родительского окна</i>
	LPARAM lParam 	<i>// определяемое программой значение</i>
);
</pre>
<p>Параметры
<p><b>hwnd</b><br>
Идендификатор окна верхнего уровня
<b>lParam</b><br>
Устанавливает определяемое программой значение передаваемое в <a href=r.php?67>EnumWindows</a> или <b>EnumDesktopWindows</b>.
<p>Возвращаемые значения<br>
Чтобы продолжить перечисление, функция повторного вызова должна возвратить значение ИСТИНА (<b>TRUE</b>); чтобы остановить перечисление, она должна возвратить - ЛОЖЬ (<b>FALSE</b>).
<p>Замечания<br>
Функция повторного вызова может выполнять любую желаемую задачу. Прикладная программа должна зарегистрировать эту функцию повторного вызова,  передавая адрес в <a href=r.php?67>EnumWindows</a> или <b>EnumDesktopWindows</b>. <b>EnumWindowsProc</b> - метка - заполнитель для определяемого программой имени функции. Тип <b>WNDENUMPROC</b> - указатель на функцию <b>EnumWindowsProc</b>.
<p>Смотри также</br>
<a href=r.php?67>EnumWindows</a>, EnumDesktopWindows
<p>Размещение и совместимость <b>EnumWindowsProc</b>
<pre>
Windows NT			Да
Win95				Да
Win32s				Да
Импортируемая библиотека		определяется пользователем
Заголовочный файл			winuser.h
Unicode				Нет
Замечания по платформе		WNDENUMPROC
</pre></div><center><hr><a href=r.php?67>Предыдущий Шаг</a> | <a href=r.php?69>Следующий Шаг</a> | <a href=apiwind2.html>Оглавление</a><br>Автор <a href="mailto:vladsok@mail.vntc.ru?SUBJECT=From_part_apiwind#68'_on_[www.firststeps.ru]">Владимир Соковиков</a>.<hr><script async src="//pagead2.googlesyndication.com/pagead/js/adsbygoogle.js"></script>
<!-- page top adaptive -->
<ins class="adsbygoogle"
     style="display:block"
     data-ad-client="ca-pub-2721325097469880"
     data-ad-slot="2551301440"
     data-ad-format="auto"
     data-full-width-responsive="true"></ins>
<script>
(adsbygoogle = window.adsbygoogle || []).push({});
</script></center><br><br>