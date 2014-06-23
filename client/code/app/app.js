// ====== Request Handling ====== //

// == Live Webcam Stream == //

navigator.getUserMedia  = navigator.getUserMedia       ||
													navigator.webkitGetUserMedia ||
													navigator.mozGetUserMedia    ||
													navigator.msGetUserMedia

navigator.getUserMedia(
	{  // Constraints
		audio: false,
		video: true
	},
	function(localMediaStream) {  // Success callback
		var videoStream = document.querySelector('video')
		videoStream.src = window.URL.createObjectURL(localMediaStream)
	},
	function(err) {  // Error callback
		console.log("The following error occured: " + err)
	}
)

// == Rendered Canvas Stream == //

timer = setInterval (
	function() {
		ctx.drawImage(videoFrame, 0, 0, 640, 480)
		var data = canvasFrame.toDataURL('image/jpeg', 1.0)
		newblob = dataURItoBlob(data)
		exports.sendImage(newblob, function(imageSize) {
			console.log('Response text:' + imageSize)
		})
	},
	1000
)

exports.sendImage = function(image, cb) { 
		return ss.rpc('server.sendBlob', image, cb)
}

function dataURItoBlob(dataURI) {
		var binary = atob(dataURI.split(',')[1])
		var array = []
		for(var i = 0; i < binary.length; i++) {
				array.push(binary.charCodeAt(i))
		}
		return new Blob([new Uint8Array(array)], {type: 'image/jpeg'})
}

var videoFrame = document.getElementById('videoStream')

var canvasFrame = document.getElementById('canvasStream')

var ctx = canvasFrame.getContext('2d')

// ====== Response Handling ====== //

ss.event.on('Image Size', function(response) {  // Listen out for newMessage events coming from the server
	console.log('Client received response: ' + response)
})
