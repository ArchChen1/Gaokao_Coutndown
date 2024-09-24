var wait = 1;//0涓嶇瓑寰呭姞杞斤紝1绛夊緟鍔犺浇
var time = 800;
var timeout = 3000;
document.onreadystatechange = function () {
    //瓒呮椂鍏抽棴
    setTimeout(function () {
        $(".loaderbg").fadeOut(time);
    }, timeout);
    //涓荤▼搴
    if (wait == 0) {
        $(".loaderbg").hide();
    } else if (document.readyState == "complete" && wait == 1) {
        $(".loaderbg").fadeOut(time);
    } else {
        $(".loaderbg").show();
    }

}