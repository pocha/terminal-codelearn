$("input[name='command']").idleTimer(5*60*1000);
$("input[name='command']").on('idle.idleTimer',function(){
	client1.close();
});

