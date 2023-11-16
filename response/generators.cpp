/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generators.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azari <azari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 15:01:25 by azari             #+#    #+#             */
/*   Updated: 2023/11/13 15:07:55 by azari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

std::string Response::generateStatusPhrase(size_t status){
	
	std::string phrase;

	switch (status)
	{
		case 100:
			phrase = " 100 Continue";
			break;
		case 101:
			phrase = " 101 Switching Protocols";
			break;
		case 200:
			phrase = " 200 OK";
			break;
		case 201:
			phrase = " 201 Created";
			break;
		case 202:
			phrase = " 202 Accepted";
			break;
		case 203:
			phrase = " 203 Non-Authoritative Information";
			break;
		case 204:
			phrase = " 204 No Content";
			break;
		case 205:
			phrase = " 205 Reset Content";
			break;
		case 206:
			phrase = " 206 Partial Content";
			break;
		case 300:
			phrase = " 300 Multiple Choices";
			break;
		case 301:
			phrase = " 301 Moved Permanently";
			break;
		case 302:
			phrase = " 302 Found";
			break;
		case 303:
			phrase = " 303 See Other";
			break;
		case 304:
			phrase = " 304 Not Modified";
			break;
		case 305:
			phrase = " 305 Use Proxy";
			break;
		case 307:
			phrase = " 307 Temporary Redirect";
			break;
		case 400:
			phrase = " 400 Bad Request";
			break;
		case 401:
			phrase = " 401 Unauthorized";
			break;
		case 402:
			phrase = " 402 Payment Required";
			break;
		case 403:
			phrase = " 403 Forbidden";
			break;
		case 404:
			phrase = " 404 Not Found";
			break;
		case 405:
			phrase = " 405 Method Not Allowed";
			break;
		case 406:
			phrase = " 406 Not Acceptable";
			break;
		case 407:
			phrase = " 407 Proxy Authentication Required";
			break;
		case 408:
			phrase = " 408 Request Time-out";
			break;
		case 409:
			phrase = " 409 Conflict";
			break;
		case 410:
			phrase = " 410 Gone";
			break;
		case 411:
			phrase = " 411 Length Required";
			break;
		case 412:
			phrase = " 412 Precondition Failed";
			break;
		case 413:
			phrase = " 413 Request Entity Too Large";
			break;
		case 414:
			phrase = " 414 Request-URI Too Large";
			break;
		case 415:
			phrase = " 415 Unsupported Media Type";
			break;
		case 416:
			phrase = " 416 Requested range not satisfiable";
			break;
		case 417:
			phrase = " 417 Expectation Failed";
			break;
		case 500:
			phrase = " 500 Internal Server Error";
			break;
		case 501:
			phrase = " 501 Not Implemented";
			break;
		case 502:
			phrase = " 502 Bad Gateway";
			break;
		case 503:
			phrase = " 503 Service Unavailable";
			break;
		case 504:
			phrase = " 504 Gateway Time-out";
			break;
		case 505:
			phrase = " 505 HTTP Version not supported";
			break;
		default:
			phrase = " 200 OK";
			break;
	}
	return phrase;
}

std::string Response::findMimeType(std::string extention)
{
    std::map<std::string, std::string> mime_types;

    mime_types[".323"] = "text/h323";
    mime_types[".3g2"] = "video/3gpp2";
    mime_types[".3gp"] = "video/3gpp";
    mime_types[".3gp2"] = "video/3gpp2";
    mime_types[".3gpp"] = "video/3gpp";
    mime_types[".7z"] = "application/x-7z-compressed";
    mime_types[".aa"] = "audio/audible";
    mime_types[".AAC"] = "audio/aac";
    mime_types[".aaf"] = "application/octet-stream";
    mime_types[".aax"] = "audio/vnd.audible.aax";
    mime_types[".ac3"] = "audio/ac3";
    mime_types[".aca"] = "application/octet-stream";
    mime_types[".accda"] = "application/msaccess.addin";
    mime_types[".accdb"] = "application/msaccess";
    mime_types[".accdc"] = "application/msaccess.cab";
    mime_types[".accde"] = "application/msaccess";
    mime_types[".accdr"] = "application/msaccess.runtime";
    mime_types[".accdt"] = "application/msaccess";
    mime_types[".accdw"] = "application/msaccess.webapplication";
    mime_types[".accft"] = "application/msaccess.ftemplate";
    mime_types[".acx"] = "application/internet-property-stream";
    mime_types[".AddIn"] = "text/xml";
    mime_types[".ade"] = "application/msaccess";
    mime_types[".adobebridge"] = "application/x-bridge-url";
    mime_types[".adp"] = "application/msaccess";
    mime_types[".ADT"] = "audio/vnd.dlna.adts";
    mime_types[".ADTS"] = "audio/aac";
    mime_types[".afm"] = "application/octet-stream";
    mime_types[".ai"] = "application/postscript";
    mime_types[".aif"] = "audio/aiff";
    mime_types[".aifc"] = "audio/aiff";
    mime_types[".aiff"] = "audio/aiff";
    mime_types[".air"] = "application/vnd.adobe.air-application-installer-package+zip";
    mime_types[".amc"] = "application/mpeg";
    mime_types[".anx"] = "application/annodex";
    mime_types[".apk"] = "application/vnd.android.package-archive";
    mime_types[".apng"] = "image/apng";
    mime_types[".application"] = "application/x-ms-application";
    mime_types[".art"] = "image/x-jg";
    mime_types[".asa"] = "application/xml";
    mime_types[".asax"] = "application/xml";
    mime_types[".ascx"] = "application/xml";
    mime_types[".asd"] = "application/octet-stream";
    mime_types[".asf"] = "video/x-ms-asf";
    mime_types[".ashx"] = "application/xml";
    mime_types[".asi"] = "application/octet-stream";
    mime_types[".asm"] = "text/plain";
    mime_types[".asmx"] = "application/xml";
    mime_types[".aspx"] = "application/xml";
    mime_types[".asr"] = "video/x-ms-asf";
    mime_types[".asx"] = "video/x-ms-asf";
    mime_types[".atom"] = "application/atom+xml";
    mime_types[".au"] = "audio/basic";
    mime_types[".avci"] = "image/avci";
    mime_types[".avcs"] = "image/avcs";
    mime_types[".avi"] = "video/x-msvideo";
    mime_types[".avif"] = "image/avif";
    mime_types[".avifs"] = "image/avif-sequence";
    mime_types[".axa"] = "audio/annodex";
    mime_types[".axs"] = "application/olescript";
    mime_types[".axv"] = "video/annodex";
    mime_types[".bas"] = "text/plain";
    mime_types[".bcpio"] = "application/x-bcpio";
    mime_types[".bin"] = "application/octet-stream";
    mime_types[".bmp"] = "image/bmp";
    mime_types[".c"] = "text/plain";
    mime_types[".cab"] = "application/octet-stream";
    mime_types[".caf"] = "audio/x-caf";
    mime_types[".calx"] = "application/vnd.ms-office.calx";
    mime_types[".cat"] = "application/vnd.ms-pki.seccat";
    mime_types[".cc"] = "text/plain";
    mime_types[".cd"] = "text/plain";
    mime_types[".cdda"] = "audio/aiff";
    mime_types[".cdf"] = "application/x-cdf";
    mime_types[".cer"] = "application/x-x509-ca-cert";
    mime_types[".cfg"] = "text/plain";
    mime_types[".chm"] = "application/octet-stream";
    mime_types[".class"] = "application/x-java-applet";
    mime_types[".clp"] = "application/x-msclip";
    mime_types[".cmd"] = "text/plain";
    mime_types[".cmx"] = "image/x-cmx";
    mime_types[".cnf"] = "text/plain";
    mime_types[".cod"] = "image/cis-cod";
    mime_types[".config"] = "application/xml";
    mime_types[".contact"] = "text/x-ms-contact";
    mime_types[".coverage"] = "application/xml";
    mime_types[".cpio"] = "application/x-cpio";
    mime_types[".cpp"] = "text/plain";
    mime_types[".crd"] = "application/x-mscardfile";
    mime_types[".crl"] = "application/pkix-crl";
    mime_types[".crt"] = "application/x-x509-ca-cert";
    mime_types[".cs"] = "text/plain";
    mime_types[".csdproj"] = "text/plain";
    mime_types[".csh"] = "application/x-csh";
    mime_types[".csproj"] = "text/plain";
    mime_types[".css"] = "text/css";
    mime_types[".csv"] = "text/csv";
    mime_types[".cur"] = "application/octet-stream";
    mime_types[".czx"] = "application/x-czx";
    mime_types[".cxx"] = "text/plain";
    mime_types[".dat"] = "application/octet-stream";
    mime_types[".datasource"] = "application/xml";
    mime_types[".dbproj"] = "text/plain";
    mime_types[".dcr"] = "application/x-director";
    mime_types[".def"] = "text/plain";
    mime_types[".deploy"] = "application/octet-stream";
    mime_types[".der"] = "application/x-x509-ca-cert";
    mime_types[".dgml"] = "application/xml";
    mime_types[".dib"] = "image/bmp";
    mime_types[".dif"] = "video/x-dv";
    mime_types[".dir"] = "application/x-director";
    mime_types[".disco"] = "text/xml";
    mime_types[".divx"] = "video/divx";
    mime_types[".dll"] = "application/x-msdownload";
    mime_types[".dll.config"] = "text/xml";
    mime_types[".dlm"] = "text/dlm";
    mime_types[".doc"] = "application/msword";
    mime_types[".docm"] = "application/vnd.ms-word.document.macroEnabled.12";
    mime_types[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    mime_types[".dot"] = "application/msword";
    mime_types[".dotm"] = "application/vnd.ms-word.template.macroEnabled.12";
    mime_types[".dotx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.template";
    mime_types[".dsp"] = "application/octet-stream";
    mime_types[".dsw"] = "text/plain";
    mime_types[".dtd"] = "text/xml";
    mime_types[".dtsConfig"] = "text/xml";
    mime_types[".dv"] = "video/x-dv";
    mime_types[".dvi"] = "application/x-dvi";
    mime_types[".dwf"] = "drawing/x-dwf";
    mime_types[".dwg"] = "application/acad";
    mime_types[".dwp"] = "application/octet-stream";
    mime_types[".dxf"] = "application/x-dxf";
    mime_types[".dxr"] = "application/x-director";
    mime_types[".eml"] = "message/rfc822";
    mime_types[".emf"] = "image/emf";
    mime_types[".emz"] = "application/octet-stream";
    mime_types[".eot"] = "application/vnd.ms-fontobject";
    mime_types[".eps"] = "application/postscript";
    mime_types[".es"] = "application/ecmascript";
    mime_types[".etl"] = "application/etl";
    mime_types[".etx"] = "text/x-setext";
    mime_types[".evy"] = "application/envoy";
    mime_types[".exe"] = "application/x-msdos-program";
    mime_types[".exe.config"] = "text/xml";
    mime_types[".f4v"] = "video/mp4";
    mime_types[".fdf"] = "application/vnd.fdf";
    mime_types[".fif"] = "application/fractals";
    mime_types[".filters"] = "application/xml";
    mime_types[".fla"] = "application/octet-stream";
    mime_types[".flac"] = "audio/flac";
    mime_types[".flr"] = "x-world/x-vrml";
    mime_types[".flv"] = "video/x-flv";
    mime_types[".fsscript"] = "application/fsharp-script";
    mime_types[".fsx"] = "application/fsharp-script";
    mime_types[".generictest"] = "application/xml";
    mime_types[".geojson"] = "application/geo+json";
    mime_types[".gif"] = "image/gif";
    mime_types[".gpx"] = "application/gpx+xml";
    mime_types[".group"] = "text/x-ms-group";
    mime_types[".gsm"] = "audio/x-gsm";
    mime_types[".gtar"] = "application/x-gtar";
    mime_types[".gz"] = "application/x-gzip";
    mime_types[".h"] = "text/plain";
    mime_types[".hdf"] = "application/x-hdf";
    mime_types[".hdml"] = "text/x-hdml";
    mime_types[".heic"] = "image/heic";
    mime_types[".heics"] = "image/heic-sequence";
    mime_types[".heif"] = "image/heif";
    mime_types[".heifs"] = "image/heif-sequence";
    mime_types[".hhc"] = "application/x-oleobject";
    mime_types[".hhk"] = "application/octet-stream";
    mime_types[".hhp"] = "application/octet-stream";
    mime_types[".hlp"] = "application/winhlp";
    mime_types[".hpp"] = "text/plain";
    mime_types[".hqx"] = "application/mac-binhex40";
    mime_types[".hta"] = "application/hta";
    mime_types[".htc"] = "text/x-component";
    mime_types[".htm"] = "text/html";
    mime_types[".html"] = "text/html";
    mime_types[".htt"] = "text/webviewhtml";
    mime_types[".hxa"] = "application/xml";
    mime_types[".hxc"] = "application/xml";
    mime_types[".hxd"] = "application/octet-stream";
    mime_types[".hxe"] = "application/xml";
    mime_types[".hxf"] = "application/xml";
    mime_types[".hxh"] = "application/octet-stream";
    mime_types[".hxi"] = "application/octet-stream";
    mime_types[".hxk"] = "application/xml";
    mime_types[".hxq"] = "application/octet-stream";
    mime_types[".hxr"] = "application/octet-stream";
    mime_types[".hxs"] = "application/octet-stream";
    mime_types[".hxt"] = "text/html";
    mime_types[".hxv"] = "application/xml";
    mime_types[".hxw"] = "application/octet-stream";
    mime_types[".hxx"] = "text/plain";
    mime_types[".i"] = "text/plain";
    mime_types[".ical"] = "text/calendar";
    mime_types[".icalendar"] = "text/calendar";
    mime_types[".ico"] = "image/x-icon";
    mime_types[".ics"] = "text/calendar";
    mime_types[".idl"] = "text/plain";
    mime_types[".ief"] = "image/ief";
    mime_types[".ifb"] = "text/calendar";
    mime_types[".iii"] = "application/x-iphone";
    mime_types[".inc"] = "text/plain";
    mime_types[".inf"] = "application/octet-stream";
    mime_types[".ini"] = "text/plain";
    mime_types[".inl"] = "text/plain";
    mime_types[".ins"] = "application/x-internet-signup";
    mime_types[".ipa"] = "application/x-itunes-ipa";
    mime_types[".ipg"] = "application/x-itunes-ipg";
    mime_types[".ipproj"] = "text/plain";
    mime_types[".ipsw"] = "application/x-itunes-ipsw";
    mime_types[".iqy"] = "text/x-ms-iqy";
    mime_types[".isp"] = "application/x-internet-signup";
    mime_types[".isma"] = "application/octet-stream";
    mime_types[".ismv"] = "application/octet-stream";
    mime_types[".ite"] = "application/x-itunes-ite";
    mime_types[".itlp"] = "application/x-itunes-itlp";
    mime_types[".itms"] = "application/x-itunes-itms";
    mime_types[".itpc"] = "application/x-itunes-itpc";
    mime_types[".IVF"] = "video/x-ivf";
    mime_types[".jar"] = "application/java-archive";
    mime_types[".java"] = "application/octet-stream";
    mime_types[".jck"] = "application/liquidmotion";
    mime_types[".jcz"] = "application/liquidmotion";
    mime_types[".jfif"] = "image/pjpeg";
    mime_types[".jnlp"] = "application/x-java-jnlp-file";
    mime_types[".jpb"] = "application/octet-stream";
    mime_types[".jpe"] = "image/jpeg";
    mime_types[".jpeg"] = "image/jpeg";
    mime_types[".jpg"] = "image/jpeg";
    mime_types[".js"] = "application/javascript";
    mime_types[".json"] = "application/json";
    mime_types[".jsx"] = "text/jscript";
    mime_types[".jsxbin"] = "text/plain";
    mime_types[".key"] = "application/x-iwork-keynote-sffkey";
    mime_types[".latex"] = "application/x-latex";
    mime_types[".library-ms"] = "application/windows-library+xml";
    mime_types[".lit"] = "application/x-ms-reader";
    mime_types[".loadtest"] = "application/xml";
    mime_types[".lpk"] = "application/octet-stream";
    mime_types[".lsf"] = "video/x-la-asf";
    mime_types[".lst"] = "text/plain";
    mime_types[".lsx"] = "video/x-la-asf";
    mime_types[".lzh"] = "application/octet-stream";
    mime_types[".m13"] = "application/x-msmediaview";
    mime_types[".m14"] = "application/x-msmediaview";
    mime_types[".m1v"] = "video/mpeg";
    mime_types[".m2t"] = "video/vnd.dlna.mpeg-tts";
    mime_types[".m2ts"] = "video/vnd.dlna.mpeg-tts";
    mime_types[".m2v"] = "video/mpeg";
    mime_types[".m3u"] = "audio/x-mpegurl";
    mime_types[".m3u8"] = "audio/x-mpegurl";
    mime_types[".m4a"] = "audio/x-m4a";
    mime_types[".m4b"] = "audio/m4b";
    mime_types[".m4p"] = "audio/m4p";
    mime_types[".m4r"] = "audio/x-m4r";
    mime_types[".m4v"] = "video/x-m4v";
    mime_types[".mac"] = "image/x-macpaint";
    mime_types[".mak"] = "text/plain";
    mime_types[".man"] = "application/x-troff-man";
    mime_types[".manifest"] = "application/x-ms-manifest";
    mime_types[".map"] = "text/plain";
    mime_types[".master"] = "application/xml";
    mime_types[".mbox"] = "application/mbox";
    mime_types[".mda"] = "application/msaccess";
    mime_types[".mdb"] = "application/x-msaccess";
    mime_types[".mde"] = "application/msaccess";
    mime_types[".mdp"] = "application/octet-stream";
    mime_types[".me"] = "application/x-troff-me";
    mime_types[".mfp"] = "application/x-shockwave-flash";
    mime_types[".mht"] = "message/rfc822";
    mime_types[".mhtml"] = "message/rfc822";
    mime_types[".mid"] = "audio/mid";
    mime_types[".midi"] = "audio/mid";
    mime_types[".mix"] = "application/octet-stream";
    mime_types[".mk"] = "text/plain";
    mime_types[".mk3d"] = "video/x-matroska-3d";
    mime_types[".mka"] = "audio/x-matroska";
    mime_types[".mkv"] = "video/x-matroska";
    mime_types[".mmf"] = "application/x-smaf";
    mime_types[".mno"] = "text/xml";
    mime_types[".mny"] = "application/x-msmoney";
    mime_types[".mod"] = "video/mpeg";
    mime_types[".mov"] = "video/quicktime";
    mime_types[".movie"] = "video/x-sgi-movie";
    mime_types[".mp2"] = "video/mpeg";
    mime_types[".mp2v"] = "video/mpeg";
    mime_types[".mp3"] = "audio/mpeg";
    mime_types[".mp4"] = "video/mp4";
    mime_types[".mp4v"] = "video/mp4";
    mime_types[".mpa"] = "video/mpeg";
    mime_types[".mpe"] = "video/mpeg";
    mime_types[".mpeg"] = "video/mpeg";
    mime_types[".mpf"] = "application/vnd.ms-mediapackage";
    mime_types[".mpg"] = "video/mpeg";
    mime_types[".mpp"] = "application/vnd.ms-project";
    mime_types[".mpv2"] = "video/mpeg";
    mime_types[".mqv"] = "video/quicktime";
    mime_types[".ms"] = "application/x-troff-ms";
    mime_types[".msg"] = "application/vnd.ms-outlook";
    mime_types[".msi"] = "application/octet-stream";
    mime_types[".mso"] = "application/octet-stream";
    mime_types[".mts"] = "video/vnd.dlna.mpeg-tts";
    mime_types[".mtx"] = "application/xml";
    mime_types[".mvb"] = "application/x-msmediaview";
    mime_types[".mvc"] = "application/x-miva-compiled";
    mime_types[".mxf"] = "application/mxf";
    mime_types[".mxp"] = "application/x-mmxp";
    mime_types[".nc"] = "application/x-netcdf";
    mime_types[".nsc"] = "video/x-ms-asf";
    mime_types[".numbers"] = "application/x-iwork-numbers-sffnumbers";
    mime_types[".nws"] = "message/rfc822";
    mime_types[".ocx"] = "application/octet-stream";
    mime_types[".oda"] = "application/oda";
    mime_types[".odb"] = "application/vnd.oasis.opendocument.database";
    mime_types[".odc"] = "application/vnd.oasis.opendocument.chart";
    mime_types[".odf"] = "application/vnd.oasis.opendocument.formula";
    mime_types[".odg"] = "application/vnd.oasis.opendocument.graphics";
    mime_types[".odh"] = "text/plain";
    mime_types[".odi"] = "application/vnd.oasis.opendocument.image";
    mime_types[".odl"] = "text/plain";
    mime_types[".odm"] = "application/vnd.oasis.opendocument.text-master";
    mime_types[".odp"] = "application/vnd.oasis.opendocument.presentation";
    mime_types[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    mime_types[".odt"] = "application/vnd.oasis.opendocument.text";
    mime_types[".oga"] = "audio/ogg";
    mime_types[".ogg"] = "audio/ogg";
    mime_types[".ogv"] = "video/ogg";
    mime_types[".ogx"] = "application/ogg";
    mime_types[".one"] = "application/onenote";
    mime_types[".onea"] = "application/onenote";
    mime_types[".onepkg"] = "application/onenote";
    mime_types[".onetmp"] = "application/onenote";
    mime_types[".onetoc"] = "application/onenote";
    mime_types[".onetoc2"] = "application/onenote";
    mime_types[".opus"] = "audio/ogg; codecs=opus";
    mime_types[".orderedtest"] = "application/xml";
    mime_types[".osdx"] = "application/opensearchdescription+xml";
    mime_types[".otf"] = "application/font-sfnt";
    mime_types[".otg"] = "application/vnd.oasis.opendocument.graphics-template";
    mime_types[".oth"] = "application/vnd.oasis.opendocument.text-web";
    mime_types[".otp"] = "application/vnd.oasis.opendocument.presentation-template";
    mime_types[".ots"] = "application/vnd.oasis.opendocument.spreadsheet-template";
    mime_types[".ott"] = "application/vnd.oasis.opendocument.text-template";
    mime_types[".oxps"] = "application/oxps";
    mime_types[".oxt"] = "application/vnd.openofficeorg.extension";
    mime_types[".p10"] = "application/pkcs10";
    mime_types[".p12"] = "application/x-pkcs12";
    mime_types[".p7b"] = "application/x-pkcs7-certificates";
    mime_types[".p7c"] = "application/pkcs7-mime";
    mime_types[".p7m"] = "application/pkcs7-mime";
    mime_types[".p7r"] = "application/x-pkcs7-certreqresp";
    mime_types[".p7s"] = "application/pkcs7-signature";
    mime_types[".pages"] = "application/x-iwork-pages-sffpages";
    mime_types[".pbm"] = "image/x-portable-bitmap";
    mime_types[".pcast"] = "application/x-podcast";
    mime_types[".pct"] = "image/pict";
    mime_types[".pcx"] = "application/octet-stream";
    mime_types[".pcz"] = "application/octet-stream";
    mime_types[".pdf"] = "application/pdf";
    mime_types[".pfb"] = "application/octet-stream";
    mime_types[".pfm"] = "application/octet-stream";
    mime_types[".pfx"] = "application/x-pkcs12";
    mime_types[".pgm"] = "image/x-portable-graymap";
    mime_types[".pic"] = "image/pict";
    mime_types[".pict"] = "image/pict";
    mime_types[".pkgdef"] = "text/plain";
    mime_types[".pkgundef"] = "text/plain";
    mime_types[".pko"] = "application/vnd.ms-pki.pko";
    mime_types[".pls"] = "audio/scpls";
    mime_types[".pma"] = "application/x-perfmon";
    mime_types[".pmc"] = "application/x-perfmon";
    mime_types[".pml"] = "application/x-perfmon";
    mime_types[".pmr"] = "application/x-perfmon";
    mime_types[".pmw"] = "application/x-perfmon";
    mime_types[".png"] = "image/x-png";
    mime_types[".pnm"] = "image/x-portable-anymap";
    mime_types[".pnt"] = "image/x-macpaint";
    mime_types[".pntg"] = "image/x-macpaint";
    mime_types[".pnz"] = "image/png";
    mime_types[".pot"] = "application/vnd.ms-powerpoint";
    mime_types[".potm"] = "application/vnd.ms-powerpoint.template.macroEnabled.12";
    mime_types[".potx"] = "application/vnd.openxmlformats-officedocument.presentationml.template";
    mime_types[".ppa"] = "application/vnd.ms-powerpoint";
    mime_types[".ppam"] = "application/vnd.ms-powerpoint.addin.macroEnabled.12";
    mime_types[".ppm"] = "image/x-portable-pixmap";
    mime_types[".pps"] = "application/vnd.ms-powerpoint";
    mime_types[".ppsm"] = "application/vnd.ms-powerpoint.slideshow.macroEnabled.12";
    mime_types[".ppsx"] = "application/vnd.openxmlformats-officedocument.presentationml.slideshow";
    mime_types[".ppt"] = "application/vnd.ms-powerpoint";
    mime_types[".pptm"] = "application/vnd.ms-powerpoint.presentation.macroEnabled.12";
    mime_types[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    mime_types[".prf"] = "application/pics-rules";
    mime_types[".prm"] = "application/octet-stream";
    mime_types[".prx"] = "application/octet-stream";
    mime_types[".ps"] = "application/postscript";
    mime_types[".psc1"] = "application/PowerShell";
    mime_types[".psd"] = "application/octet-stream";
    mime_types[".psess"] = "application/xml";
    mime_types[".psm"] = "application/octet-stream";
    mime_types[".psp"] = "application/octet-stream";
    mime_types[".pst"] = "application/vnd.ms-outlook";
    mime_types[".pub"] = "application/x-mspublisher";
    mime_types[".pwz"] = "application/vnd.ms-powerpoint";
    mime_types[".qht"] = "text/x-html-insertion";
    mime_types[".qhtm"] = "text/x-html-insertion";
    mime_types[".qt"] = "video/quicktime";
    mime_types[".qti"] = "image/x-quicktime";
    mime_types[".qtif"] = "image/x-quicktime";
    mime_types[".qtl"] = "application/x-quicktimeplayer";
    mime_types[".qxd"] = "application/octet-stream";
    mime_types[".ra"] = "audio/x-pn-realaudio";
    mime_types[".ram"] = "audio/x-pn-realaudio";
    mime_types[".rar"] = "application/x-rar-compressed";
    mime_types[".ras"] = "image/x-cmu-raster";
    mime_types[".rat"] = "application/rat-file";
    mime_types[".rc"] = "text/plain";
    mime_types[".rc2"] = "text/plain";
    mime_types[".rct"] = "text/plain";
    mime_types[".rdlc"] = "application/xml";
    mime_types[".reg"] = "text/plain";
    mime_types[".resx"] = "application/xml";
    mime_types[".rf"] = "image/vnd.rn-realflash";
    mime_types[".rgb"] = "image/x-rgb";
    mime_types[".rgs"] = "text/plain";
    mime_types[".rm"] = "application/vnd.rn-realmedia";
    mime_types[".rmi"] = "audio/mid";
    mime_types[".rmp"] = "application/vnd.rn-rn_music_package";
    mime_types[".rmvb"] = "application/vnd.rn-realmedia-vbr";
    mime_types[".roff"] = "application/x-troff";
    mime_types[".rpm"] = "audio/x-pn-realaudio-plugin";
    mime_types[".rqy"] = "text/x-ms-rqy";
    mime_types[".rtf"] = "application/rtf";
    mime_types[".rtx"] = "text/richtext";
    mime_types[".rvt"] = "application/octet-stream";
    mime_types[".ruleset"] = "application/xml";
    mime_types[".s"] = "text/plain";
    mime_types[".safariextz"] = "application/x-safari-safariextz";
    mime_types[".scd"] = "application/x-msschedule";
    mime_types[".scr"] = "text/plain";
    mime_types[".sct"] = "text/scriptlet";
    mime_types[".sd2"] = "audio/x-sd2";
    mime_types[".sdp"] = "application/sdp";
    mime_types[".sea"] = "application/octet-stream";
    mime_types[".searchConnector-ms"] = "application/windows-search-connector+xml";
    mime_types[".setpay"] = "application/set-payment-initiation";
    mime_types[".setreg"] = "application/set-registration-initiation";
    mime_types[".settings"] = "application/xml";
    mime_types[".sgimb"] = "application/x-sgimb";
    mime_types[".sgml"] = "text/sgml";
    mime_types[".sh"] = "application/x-sh";
    mime_types[".shar"] = "application/x-shar";
    mime_types[".shtml"] = "text/html";
    mime_types[".sit"] = "application/x-stuffit";
    mime_types[".sitemap"] = "application/xml";
    mime_types[".skin"] = "application/xml";
    mime_types[".skp"] = "application/x-koan";
    mime_types[".sldm"] = "application/vnd.ms-powerpoint.slide.macroEnabled.12";
    mime_types[".sldx"] = "application/vnd.openxmlformats-officedocument.presentationml.slide";
    mime_types[".slk"] = "application/vnd.ms-excel";
    mime_types[".sln"] = "text/plain";
    mime_types[".slupkg-ms"] = "application/x-ms-license";
    mime_types[".smd"] = "audio/x-smd";
    mime_types[".smi"] = "application/octet-stream";
    mime_types[".smx"] = "audio/x-smd";
    mime_types[".smz"] = "audio/x-smd";
    mime_types[".snd"] = "audio/basic";
    mime_types[".snippet"] = "application/xml";
    mime_types[".snp"] = "application/octet-stream";
    mime_types[".sql"] = "application/sql";
    mime_types[".sol"] = "text/plain";
    mime_types[".sor"] = "text/plain";
    mime_types[".spc"] = "application/x-pkcs7-certificates";
    mime_types[".spl"] = "application/futuresplash";
    mime_types[".spx"] = "audio/ogg";
    mime_types[".src"] = "application/x-wais-source";
    mime_types[".srf"] = "text/plain";
    mime_types[".SSISDeploymentManifest"] = "text/xml";
    mime_types[".ssm"] = "application/streamingmedia";
    mime_types[".sst"] = "application/vnd.ms-pki.certstore";
    mime_types[".stl"] = "application/vnd.ms-pki.stl";
    mime_types[".sv4cpio"] = "application/x-sv4cpio";
    mime_types[".sv4crc"] = "application/x-sv4crc";
    mime_types[".svc"] = "application/xml";
    mime_types[".svg"] = "image/svg+xml";
    mime_types[".swf"] = "application/x-shockwave-flash";
    mime_types[".step"] = "application/step";
    mime_types[".stp"] = "application/step";
    mime_types[".t"] = "application/x-troff";
    mime_types[".tar"] = "application/x-tar";
    mime_types[".tcl"] = "application/x-tcl";
    mime_types[".testrunconfig"] = "application/xml";
    mime_types[".testsettings"] = "application/xml";
    mime_types[".tex"] = "application/x-tex";
    mime_types[".texi"] = "application/x-texinfo";
    mime_types[".texinfo"] = "application/x-texinfo";
    mime_types[".tgz"] = "application/x-compressed";
    mime_types[".thmx"] = "application/vnd.ms-officetheme";
    mime_types[".thn"] = "application/octet-stream";
    mime_types[".tif"] = "image/tiff";
    mime_types[".tiff"] = "image/tiff";
    mime_types[".tlh"] = "text/plain";
    mime_types[".tli"] = "text/plain";
    mime_types[".toc"] = "application/octet-stream";
    mime_types[".tr"] = "application/x-troff";
    mime_types[".trm"] = "application/x-msterminal";
    mime_types[".trx"] = "application/xml";
    mime_types[".ts"] = "video/vnd.dlna.mpeg-tts";
    mime_types[".tsv"] = "text/tab-separated-values";
    mime_types[".ttf"] = "application/font-sfnt";
    mime_types[".tts"] = "video/vnd.dlna.mpeg-tts";
    mime_types[".txt"] = "text/plain";
    mime_types[".u32"] = "application/octet-stream";
    mime_types[".uls"] = "text/iuls";
    mime_types[".user"] = "text/plain";
    mime_types[".ustar"] = "application/x-ustar";
    mime_types[".vb"] = "text/plain";
    mime_types[".vbdproj"] = "text/plain";
    mime_types[".vbk"] = "video/mpeg";
    mime_types[".vbproj"] = "text/plain";
    mime_types[".vbs"] = "text/vbscript";
    mime_types[".vcf"] = "text/x-vcard";
    mime_types[".vcproj"] = "application/xml";
    mime_types[".vcs"] = "text/plain";
    mime_types[".vcxproj"] = "application/xml";
    mime_types[".vddproj"] = "text/plain";
    mime_types[".vdp"] = "text/plain";
    mime_types[".vdproj"] = "text/plain";
    mime_types[".vdx"] = "application/vnd.ms-visio.viewer";
    mime_types[".vml"] = "text/xml";
    mime_types[".vscontent"] = "application/xml";
    mime_types[".vsct"] = "text/xml";
    mime_types[".vsd"] = "application/vnd.visio";
    mime_types[".vsi"] = "application/ms-vsi";
    mime_types[".vsix"] = "application/vsix";
    mime_types[".vsixlangpack"] = "text/xml";
    mime_types[".vsixmanifest"] = "text/xml";
    mime_types[".vsmdi"] = "application/xml";
    mime_types[".vspscc"] = "text/plain";
    mime_types[".vss"] = "application/vnd.visio";
    mime_types[".vsscc"] = "text/plain";
    mime_types[".vssettings"] = "text/xml";
    mime_types[".vssscc"] = "text/plain";
    mime_types[".vst"] = "application/vnd.visio";
    mime_types[".vstemplate"] = "text/xml";
    mime_types[".vsto"] = "application/x-ms-vsto";
    mime_types[".vsw"] = "application/vnd.visio";
    mime_types[".vsx"] = "application/vnd.visio";
    mime_types[".vtt"] = "text/vtt";
    mime_types[".vtx"] = "application/vnd.visio";
    mime_types[".wasm"] = "application/wasm";
    mime_types[".wav"] = "audio/wav";
    mime_types[".wave"] = "audio/wav";
    mime_types[".wax"] = "audio/x-ms-wax";
    mime_types[".wbk"] = "application/msword";
    mime_types[".wbmp"] = "image/vnd.wap.wbmp";
    mime_types[".wcm"] = "application/vnd.ms-works";
    mime_types[".wdb"] = "application/vnd.ms-works";
    mime_types[".wdp"] = "image/vnd.ms-photo";
    mime_types[".webarchive"] = "application/x-safari-webarchive";
    mime_types[".webm"] = "video/webm";
    mime_types[".webp"] = "image/webp";
    mime_types[".webtest"] = "application/xml";
    mime_types[".wiq"] = "application/xml";
    mime_types[".wiz"] = "application/msword";
    mime_types[".wks"] = "application/vnd.ms-works";
    mime_types[".WLMP"] = "application/wlmoviemaker";
    mime_types[".wlpginstall"] = "application/x-wlpg-detect";
    mime_types[".wlpginstall3"] = "application/x-wlpg3-detect";
    mime_types[".wm"] = "video/x-ms-wm";
    mime_types[".wma"] = "audio/x-ms-wma";
    mime_types[".wmd"] = "application/x-ms-wmd";
    mime_types[".wmf"] = "application/x-msmetafile";
    mime_types[".wml"] = "text/vnd.wap.wml";
    mime_types[".wmlc"] = "application/vnd.wap.wmlc";
    mime_types[".wmls"] = "text/vnd.wap.wmlscript";
    mime_types[".wmlsc"] = "application/vnd.wap.wmlscriptc";
    mime_types[".wmp"] = "video/x-ms-wmp";
    mime_types[".wmv"] = "video/x-ms-wmv";
    mime_types[".wmx"] = "video/x-ms-wmx";
    mime_types[".wmz"] = "application/x-ms-wmz";
    mime_types[".woff"] = "application/font-woff";
    mime_types[".woff2"] = "application/font-woff2";
    mime_types[".wpl"] = "application/vnd.ms-wpl";
    mime_types[".wps"] = "application/vnd.ms-works";
    mime_types[".wri"] = "application/x-mswrite";
    mime_types[".wrl"] = "x-world/x-vrml";
    mime_types[".wrz"] = "x-world/x-vrml";
    mime_types[".wsc"] = "text/scriptlet";
    mime_types[".wsdl"] = "text/xml";
    mime_types[".wvx"] = "video/x-ms-wvx";
    mime_types[".x"] = "application/directx";
    mime_types[".xaf"] = "x-world/x-vrml";
    mime_types[".xaml"] = "application/xaml+xml";
    mime_types[".xap"] = "application/x-silverlight-app";
    mime_types[".xbap"] = "application/x-ms-xbap";
    mime_types[".xbm"] = "image/x-xbitmap";
    mime_types[".xdr"] = "text/plain";
    mime_types[".xht"] = "application/xhtml+xml";
    mime_types[".xhtml"] = "application/xhtml+xml";
    mime_types[".xla"] = "application/vnd.ms-excel";
    mime_types[".xlam"] = "application/vnd.ms-excel.addin.macroEnabled.12";
    mime_types[".xlc"] = "application/vnd.ms-excel";
    mime_types[".xld"] = "application/vnd.ms-excel";
    mime_types[".xlk"] = "application/vnd.ms-excel";
    mime_types[".xll"] = "application/vnd.ms-excel";
    mime_types[".xlm"] = "application/vnd.ms-excel";
    mime_types[".xls"] = "application/vnd.ms-excel";
    mime_types[".xlsb"] = "application/vnd.ms-excel.sheet.binary.macroEnabled.12";
    mime_types[".xlsm"] = "application/vnd.ms-excel.sheet.macroEnabled.12";
    mime_types[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    mime_types[".xlt"] = "application/vnd.ms-excel";
    mime_types[".xltm"] = "application/vnd.ms-excel.template.macroEnabled.12";
    mime_types[".xltx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.template";
    mime_types[".xlw"] = "application/vnd.ms-excel";
    mime_types[".xml"] = "text/xml";
    mime_types[".xmp"] = "application/octet-stream";
    mime_types[".xmta"] = "application/xml";
    mime_types[".xof"] = "x-world/x-vrml";
    mime_types[".XOML"] = "text/plain";
    mime_types[".xpm"] = "image/x-xpixmap";
    mime_types[".xps"] = "application/vnd.ms-xpsdocument";
    mime_types[".xrm-ms"] = "text/xml";
    mime_types[".xsc"] = "application/xml";
    mime_types[".xsd"] = "text/xml";
    mime_types[".xsf"] = "text/xml";
    mime_types[".xsl"] = "text/xml";
    mime_types[".xslt"] = "text/xml";
    mime_types[".xsn"] = "application/octet-stream";
    mime_types[".xss"] = "application/xml";
    mime_types[".xspf"] = "application/xspf+xml";
    mime_types[".xtp"] = "application/octet-stream";
    mime_types[".xwd"] = "image/x-xwindowdump";
    mime_types[".z"] = "application/x-compress";
    mime_types[".zip"] = "application/x-zip-compressed";

    return mime_types[extention];
}

std::string Response::generateResponseDate(){
	return "00:00:00";
}
