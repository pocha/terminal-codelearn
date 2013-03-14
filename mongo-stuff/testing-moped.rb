require 'moped'

session = Moped::Session.new([ "127.0.0.1:27017" ])
session.use "test"

session.with(safe: true) do |safe|
  safe[:artists].insert(name: "Syd Vicious")
end

session[:artists].find(name: "Syd Vicious").update(:$push => { instruments: { name: "Bass" }})
