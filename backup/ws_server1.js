var WebSocketServer=require('websocket').server;
var http=require('http');
var fs=require('fs');
var mysql=require('mysql');

var server=http.createServer(function(request,response) {
        fs.readFile('websocket_s.html',function(error,data){
                console.log((new Date()) + 'Received request for' + request.url);
                response.writeHead(200,{'Content-type': 'text/html'});
                response.end(data);
        });
});
server.listen(8080,function(){
        console.log((new Date()) + 'Server is listening on port 8080');
});
wsServer=new WebSocketServer({
        httpServer:server,
        autoAcceptConnections:false
});
function originIsAllowed(origin){
        return true;
}

wsServer.on('request',function(request){
        if(!originIsAllowed(request.origin)){
                request.reject();
                console.log((new Date()) + 'Connection from origin ' + request.origin+'rejected.');
                return;
        }
        var connection = request.accept(null,request.origin);
        console.log((new Date()) + 'Connection accepted.');
        var testInterval=setInterval(function() {
        	var comysql=mysql.createConnection({
        		host:'localhost',
        		user:'root',
        		password:'catchup123',
        		database:'situp'
        	});
        	comysql.connect();
        	comysql.query('select * from sit_time order by time desc limit 1',function(error,result,fields){
        			if(error){
        				console.log(error);
        				console.log('query error');
        			}
        			else{
        				for(var i=0;i<result.length;i++){
        					//connection.sendUTF(result[i].time);
						connection.sendUTF(result[i].sit_time);
					}
				}
		});
		comysql.end();
	},3000);
        						//connection.sendUTF(result[i].sit_time);
        /*connection.on('message',function(message) {
 *                 if(message.type=='utf8'){
 *                                         console.log('Received message:' + message.utf8Data);
 *                                                                 connection.sendUTF(message.utf8Data);
 *                                                                                 }
 *                                                                                                 else if(message.type=='binary'){
 *                                                                                                                         console.log('Received Binary Message of' + message.binaryData.length+ ' bytes');
 *                                                                                                                                         }
 *                                                                                                                                                 });*/
        connection.on('close',function(reasonCode,description){
                console.log((new Date())+'Peer' + connection.remoteAddress+'disconnected.');
        });
});

