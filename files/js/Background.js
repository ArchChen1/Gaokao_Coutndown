$(document).ready(function () {
    loadBG(); // 等待网页ready再执行
});

function loadBG(){
    if (bgType === 0) {
        applyAPI(imageUrl); 
    } else if (bgType === 1) {
        document.getElementById("bgVideo").style.opacity = 1;
        const videoSource = document.getElementById('videoSource');
        videoSource.src = videoUrl;
        const bgVideo = document.getElementById('bgVideo');
        bgVideo.load();
        bgVideo.muted = false;
        if(!bgVideo.muted)
        $(document).one('click', function() {// 大部分浏览器不支持有声视频的播放
            bgVideo.play();// 无奈这里添加个click交互进行播放
        });
    } else if (bgType === 2) {
        applyAPI(ACGurl);
    } else if (bgType === 3) {
        getBingIMG(); 
    } else if (bgType === 4) {
        getSpotlightAPI(); 
    } else if (bgType === 5) {
        applyAPI(customUrl); 
    }
}

const CorsCDN = "https://proxy.cenmin.us.kg/";

function getBingIMG(){
    function getBingAPI(geturl) {
        fetch(geturl)
        .then(response => response.json())
        .then(data => {
            const image = data.images[0];
            const imageUrl = "https://www.bing.com" + image.url;  // 拼接完整的图片URL
            const img = new Image(); // 创建新的 Image 对象以监控图片加载状态
            img.src = imageUrl;
            img.onload = () => {
                document.body.style.backgroundImage = `url('${imageUrl}')`;  // 设置背景图片
                const copyright = image.title + "——" + image.copyright;
                document.getElementById("copyright").innerHTML = copyright;  // 设置版权信息
            };
            return true;
        })
        .catch(error => {
            return false;
        });
    }
    

    if(!getBingAPI("https://cn.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1&mkt=zh-CN")){
        if(!getBingAPI(CorsCDN + "https://cn.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1&mkt=zh-CN")){
            document.getElementById("copyrightTXT").innerHTML = "加载失败";
            document.body.style.backgroundImage = "https://bing.img.run/1920x1080.php";
        }
    }
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
            // 如果imageDescription2也有，则不显示
            if (imageDescription.includes("...") || imageDescription.includes("…")) {
                imageDescription = imageDescription2;
                if (imageDescription2.includes("...") || imageDescription2.includes("…")) {
                    imageDescription = "";
                }
            }
            let copyright = imageLocation + imageCopyright;
            // 在页面中展示图片和解说
            const img = new Image(); // 创建新的 Image 对象以监控图片加载状态
            img.src = SpotLightSRC;
            img.onload = () => {
                document.body.style.backgroundImage = `url('${SpotLightSRC}')`;  // 设置背景图片
            };
            document.getElementById("copyright").innerHTML = copyright;  // 设置版权信息

            return true;
        },
        error: function () {
            $.ajax({
                url: 'https://api.qzink.me/spotlight',
                type: 'GET',
                dataType: 'json',
                async: false,
                success: function (data) {
                    // Update HTML elements with API response
                    const img = new Image(); // 创建新的 Image 对象以监控图片加载状态
                    img.src = data.landscape_url;
                    img.onload = () => {
                        document.body.style.backgroundImage = `url('${data.landscape_url}')`;  // 设置背景图片
                    };
                    document.getElementById("copyright").innerHTML = data.title + data.copyright;  // 设置版权信息
                    return true;
                },
                error: function () {
                    document.getElementById("copyright").innerHTML = '加载失败';
                    return false;
                }
            });
        }
    });

}


function applyAPI(API) {
    const img = new Image(); // 创建新的图片对象
    img.src = API; // 设置图片源为 API 返回的 URL

    img.onload = () => {
        document.body.style.backgroundImage = `url('${API}')`; // 设置背景图片
    };

    img.onerror = () => {
        document.body.style.backgroundImage = `url('${imageUrl}')`; // 恢复默认背景图片
        document.getElementById("copyright").innerHTML = '获取API图片失败，可能是未联网或者是API接口失效';
    };
}


let bgType = 1; // 默认背景类型
let isVideoMute = false;
let ACGurl = "https://api.maho.cc/random-img/pc.php"; // ACG 图片 API
let customUrl = "https://www.dmoe.cc/random.php"; // 自定义 API
let imageUrl = "./files/BgFile/image/2.png"; // 图片背景
let videoUrl = "./files/BgFile/video/Bakemonogatari.mkv"; // 视频背景
