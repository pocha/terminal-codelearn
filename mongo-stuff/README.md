First time into Mongodb, dont worry, me too :). 

- Install mongodb on your linux machine
- Run ./start-mongo.sh . It will start running mongodb in background. 
> My installation of mongodb did not come with init script. So I wrote this one line bash script. To kill, you need to find the process by doing 'ps -ef | grep mongo' & then kill it
- Install moped `gem install moped`
- Run `ruby testing-moped.rb` which inserts data in test db of mongo. Get into mongo by running `mongo` & type `db.artists.find()` to see if the data got inserted. 

Congrats you are all set to use mongo now :)
