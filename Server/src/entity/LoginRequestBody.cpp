#include "../include/LoginRequestBody.h"

LoginRequestBody::LoginRequestBody(const std::string& requestBody) {
  Poco::JSON::Parser parser;
  Poco::Dynamic::Var result = parser.parse(requestBody);
  Poco::JSON::Object::Ptr jsonObject = result.extract<Poco::JSON::Object::Ptr>();

  Poco::Dynamic::Var usernameVar = jsonObject->get("username");
  username = usernameVar.convert<std::string>();

  Poco::Dynamic::Var passwordVar = jsonObject->get("password");
  password = passwordVar.convert<std::string>();
}

void LoginRequestBody::handleLogin(Poco::Net::HTTPServerResponse& response) const {

  LoginDb login;
  
  // Set the response status code and message based on login success
  if (login.checkCredentials(username, password)) {
      response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
      std::ostream& ostr = response.send();
      ostr << "Login successful!";
  } else {
      response.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
      std::ostream& ostr = response.send();
      ostr << "Login failed!";
  }
}

std::string LoginRequestBody::getUsername() const {
    return username;
}

std::string LoginRequestBody::getPassword() const {
    return password;
}