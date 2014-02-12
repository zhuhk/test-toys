var cluster = require('cluster');
var http = require('http');
var numCPUs = require('os').cpus().length;

if(cluster.isMaster){
    console.log("Master:%d", process.pid);
    //fork workers
    for (var i=0; i<numCPUs; i++) {
	cluster.fork();
    }
    
    cluster.on('exit', 
	function(worker, code, signal) {
	    console.log('worker ' + worker.process.pid + ' died');
	}
    );
} else {
    console.log("Worker:%d", process.pid);
    http.createServer( 
	function(req, res){
	    res.writeHead(200);
	    res.end("hello world\n");
	}
    ).listen(3000);
}
