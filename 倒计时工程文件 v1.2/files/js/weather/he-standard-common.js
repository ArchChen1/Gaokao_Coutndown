
(function (d) {
  var c = d.createElement('link')
  c.rel = 'stylesheet'
  c.href = './files/js/weather/he-standard.css'
  var s = d.createElement('script')
  s.src = './files/js/weather/he-standard.js'
  var sn = d.getElementsByTagName('script')[0]
  sn.parentNode.insertBefore(c, sn)
  sn.parentNode.insertBefore(s, sn)
})(document)
