## session.gemspec
#

Gem::Specification::new do |spec|
  spec.name = "session"
  spec.version = "3.1.0"
  spec.platform = Gem::Platform::RUBY
  spec.summary = "session"
  spec.description = "description: session kicks the ass"

  spec.files = ["gemspec.rb", "lib", "lib/session.rb", "LICENSE", "Rakefile", "README", "sample", "sample/bash.rb", "sample/bash.rb.out", "sample/driver.rb", "sample/session_idl.rb", "sample/session_sh.rb", "sample/sh0.rb", "sample/stdin.rb", "sample/threadtest.rb", "session.gemspec", "test", "test/session.rb"]
  spec.executables = []
  
  spec.require_path = "lib"

  spec.has_rdoc = true
  spec.test_files = "test/session.rb"
  #spec.add_dependency 'lib', '>= version'
  spec.add_dependency 'fattr'

  spec.extensions.push(*[])

  spec.rubyforge_project = "codeforpeople"
  spec.author = "Ara T. Howard"
  spec.email = "ara.t.howard@gmail.com"
  spec.homepage = "http://github.com/ahoward/session/tree/master"
end
