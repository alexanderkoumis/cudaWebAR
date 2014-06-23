// Define actions which can be called from the client using ss.rpc('demo.ACTIONNAME', param1, param2...)
exports.actions = function(req, res, ss) {
	var addon = require('./build/Release/hello')
	req.use('session');
	return {

		sendMessage: function(message) {
			if (message && message.length > 0) {         // Check for blank messages
				ss.publish.all('newMessage', message)      // Broadcast the message to everyone
				return res(true)                         // Confirm it was sent to the originating client
			} else {
				return res(false)
			}
		},
	
		sendBlob: function(image) {
			console.log('addon.hello() = ' + addon.hello())
			ss.publish.all('Image Size', 'Server Response') 
			return res('Send confirmation')
		}
		
	}
}