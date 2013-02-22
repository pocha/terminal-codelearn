require 'test_helper'

class TerminalsControllerTest < ActionController::TestCase
  test "should get get" do
    get :get
    assert_response :success
  end

  test "should get execute" do
    get :execute
    assert_response :success
  end

  test "should get kill" do
    get :kill
    assert_response :success
  end

end
