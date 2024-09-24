var backgroundCONFIG = 4; // 0纯色背景，1图片背景，2视频背景，3Bing壁纸，4Windows聚焦,5ACG壁纸，6自定义API
var soundCONFIG = 0; // 0静音，1不静音
var imageSRC = "./BgFile/image/backgroundDEFAULT.png";
var bgcolor = "#ffffff";
var videoSRC = "./BgFile/video/default1.webm";
var customAPI = "https://www.dmoe.cc/random.php";
var defaultIMG = "./BgFile/image/backgroundDEFAULT.png";


$(document).ready(function () {//jquery ready命令，比window.onload好用，能重复使用
    show();//执行一次
});

var ACGSRC = "https://api.maho.cc/random-img/pc.php";
var AlternateBingSRC = "https://www.todaybing.com/api/today/cn";

//

function show() {
    document.getElementById("Under").src = defaultIMG;

    if (backgroundCONFIG == 0) {//壁纸类型纯色
        document.getElementById("backgroundIMG").style.opacity = 0;
        document.getElementById("backgroundVIDEO").style.opacity = 0;
        document.getElementById("backgroundVIDEOhidden").style.opacity = 0;
        document.getElementById("Under").style.opacity = 0;
        document.getElementById("backgroundVIDEO").muted = "muted";
        document.body.style.backgroundColor = bgcolor;//修改body背景颜色
        console.warn(bgcolor);
    }
    else if (backgroundCONFIG == 1) {//壁纸类型图片
        document.getElementById("backgroundVIDEO").style.opacity = 0;
        document.getElementById("backgroundVIDEOhidden").style.opacity = 0;
        document.getElementById("backgroundVIDEO").muted = "muted";
        testAPI(imageSRC);
    }

    else if (backgroundCONFIG == 2) {//壁纸类型视频
        var vdo = document.getElementById("backgroundVIDEO");
        var vdoh = document.getElementById("backgroundVIDEOhidden");
        vdo.src = videoSRC;
        vdoh.src = videoSRC;
    }

    else if (backgroundCONFIG == 3) {//壁纸类型bing
        document.getElementById("backgroundVIDEO").style.opacity = 0;
        document.getElementById("backgroundVIDEOhidden").style.opacity = 0;
        document.getElementById("backgroundVIDEO").muted = "muted";
        getBingAPI();
    } else if (backgroundCONFIG == 4) {//壁纸类型Windows聚焦
        document.getElementById("backgroundVIDEO").style.opacity = 0;
        document.getElementById("backgroundVIDEOhidden").style.opacity = 0;
        document.getElementById("backgroundVIDEO").muted = "muted";
        getSpotlightAPI();
    }
    else if (backgroundCONFIG == 5) {//壁纸类型ACG
        document.getElementById("backgroundVIDEO").style.opacity = 0;
        document.getElementById("backgroundVIDEOhidden").style.opacity = 0;
        document.getElementById("backgroundVIDEO").muted = "muted";
        testAPI(ACGSRC);
    }
    else if (backgroundCONFIG == 6) {//壁纸类型自定义API
        document.getElementById("backgroundVIDEO").style.opacity = 0;
        document.getElementById("backgroundVIDEOhidden").style.opacity = 0;
        document.getElementById("backgroundVIDEO").muted = "muted";
        testAPI(customAPI);
    }
    if (soundCONFIG == 0) {//是否静音（静音）
        document.getElementById("backgroundVIDEO").muted = true;
    }
}


function getBingAPI() {
    BingAJAX = $.ajax({
        dateType: "json",
        type: "get",
        url: "https://cn.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1&mkt=zh-CN",
        async: false,
        success: function (data) {
            var BingSRC = 'https://cn.bing.com' + data.images[0].url;
            var copyright = data.images[0].copyright;
            document.getElementById("copyright").innerHTML = copyright;
            $('#backgroundIMG').attr('src', BingSRC);
        },
        error: function () {
            document.getElementById("copyright").innerHTML = '备用接口';
            testAPI(AlternateBingSRC)
        }
    }
    )
}
function getSpotlightAPI() {
    $.ajax({
        url: "https://arc.msn.com/v3/Delivery/Placement?pid=209567&fmt=json&cdm=1&pl=zh-CN&lc=zh-CN&ctry=CN",
        type: "GET",
        dataType: "json",
        async: false,
        success: function (data) {
            // 假设 data 是 JSON 数据
            var jsonDataString = data.batchrsp.items[0].item; // 获取嵌套的 JSON 字符串
            var jsonData = JSON.parse(jsonDataString); // 解析嵌套的 JSON 字符串
            // 使用 jsonData.ad 来访问ad数据
            var ad = jsonData.ad;

            var SpotLightSRC = ad.image_fullscreen_001_landscape ? ad.image_fullscreen_001_landscape.u : '';
            var imageLocation = ad.title_text ? ad.title_text.tx : '';
            var imageCopyright = ad.copyright_text ? ad.copyright_text.tx : '';
            var imageDescription = ad.hs1_title_text ? ad.hs1_title_text.tx : '';
            var imageDescription2 = ad.hs2_title_text ? ad.hs2_title_text.tx : '';

            // 判断 imageDescription 是否包含 "…"，如果是，则使用 imageDescription2
            // 如果imageDescription2也有，则不显示(以防必死强迫症)
            if (imageDescription.includes("...") || imageDescription.includes("…")) {
                imageDescription = imageDescription2;
                if (imageDescription2.includes("...") || imageDescription2.includes("…")) {
                    imageDescription = "";

                }
            }

            // 在页面中展示图片和解说
            $('#backgroundIMG').attr('src', SpotLightSRC);
            $("#copyright").text(imageLocation + imageCopyright);
            $("#copyright2").text(imageDescription);

        },
        error: function () {
            $.ajax({
                url: 'https://api.qzink.me/spotlight',
                type: 'GET',
                dataType: 'json',
                async: false,
                success: function (data) {
                    // Update HTML elements with API response
                    $('#backgroundIMG').attr('src', data.landscape_url);
                    $('#copyright').text(data.title + data.copyright);
                },
                error: function () {
                    document.getElementById("copyright").innerHTML = '获取API图片失败，可能是未联网或者是API接口失效';
                }
            });
        }
    });

}
function testAPI(API) {
    $('#backgroundIMG').attr('src', API);
    document.getElementById("backgroundIMG").addEventListener("load", () => {
        document.getElementById("Under").style.opacity = 0;
    })
    document.getElementById("backgroundIMG").addEventListener("error", () => {
        document.getElementById("copyright").innerHTML = '获取API图片失败，可能是未联网或者是API接口失效';
        $('#backgroundIMG').attr('src', '');

    })
}

