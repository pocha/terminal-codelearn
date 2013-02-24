class TerminalsController < ApplicationController
	
  def current_user
	  "pocha"
  end

  def index
  end

  def get
	  http = Curl.get("http://localhost:3001/#{current_user}/get")
	  render :json => http.body_str
  end

  def execute
	  url = "http://localhost:3001/#{current_user}/execute/#{params[:command].nil? ? "" : CGI::escape(params[:command])}" 
	  puts url
	  http = Curl.get(url)
	  render :json => http.body_str
  end

  def kill
	  http = Curl.get("http://localhost:3001/#{current_user}/kill")
	  render :text => http.body_str
  end
end
