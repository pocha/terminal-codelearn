class TerminalsController < ApplicationController
	
  def current_user
	  "pocha"
  end

  def index
  end

  def get
	  http = Curl.get("http://localhost:3001/#{current_user}/get")
	  render :text => http.body_str
  end

  def execute
	  #http = Curl.post("http://localhost:3001/#{current_user}", {:command => params[:command]})
	  http = Curl.post("http://localhost:3001/#{current_user}/execute/#{params[:command]}")
	  render :text => http.body_str
  end

  def kill
	  http = Curl.get("http://localhost:3001/#{current_user}/kill")
	  render :text => http.body_str
  end
end
