$("input[name='command']").idleTimer(60*5*1000);
$("input[name='command']").on('idle.idleTimer',timerFired);

