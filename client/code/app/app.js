// ============ Alexander code ============ //


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

// ============ Sample code ============ //


// ====== Request Handling ====== //

$('#chat').on('submit', function() { // Show the chat form and bind to the submit action
	var text = $('#myMessage').val();  // Grab the message from the text box
	return exports.sendString(text, function(success) {  // Call the 'send' funtion (below) to ensure it's valid before sending to the server
		if (success) {
			return $('#myMessage').val('')
		} else {
			return alert('Oops! Unable to send message')
		}
	})
})

exports.sendString = function(text, cb) {  // Demonstrates sharing code between modules by exporting function
	if (valid(text)) {
		return ss.rpc('server.sendMessage', text, cb)
	} else {
		return cb(false)
	}
}

var valid = function(text) {
	return text && text.length > 0
}

// ====== Response Handling ====== //

ss.event.on('newMessage', function(message) {  // Listen out for newMessage events coming from the server
	var html = ss.tmpl['chat-message'].render({  // Example of using the Hogan Template in client/templates/chat/message.jade to generate HTML for each message
		message: message,
		time: function() { return timestamp() }
	});
	return $(html).hide().appendTo('#chatlog').slideDown()  // Append it to the #chatlog div and show effect
});

var timestamp = function() {
	var d = new Date()
	return d.getHours() + ':' + pad2(d.getMinutes()) + ':' + pad2(d.getSeconds())
}

var pad2 = function(number) {
	return (number < 10 ? '0' : '') + number
}
