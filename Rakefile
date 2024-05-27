# frozen_string_literal: true

ENV["PRISM_JAVA_BACKEND"]="jruby"
ENV["PRISM_SERIALIZE_ONLY_SEMANTICS_FIELDS"]="1"


require "bundler/gem_tasks"
require "rake/clean"

task default: %i[compile test]

require_relative "templates/template"

desc "Generate all ERB template based files"
task templates: Prism::Template::TEMPLATES

make = RUBY_PLATFORM.include?("openbsd") ? "gmake" : "make"
task(make: :templates) { sh(make) }
task(make_no_debug: :templates) { sh("#{make} all-no-debug") }
task(make_minimal: :templates) { sh("#{make} minimal") }

task compile: :make
task compile_no_debug: %i[make_no_debug compile]
task compile_minimal: %i[make_minimal compile]

# decorate the gem build task with prerequisites
task build: [:check_manifest, :templates, :vendor_jar]

# the C extension
task "compile:prism" => ["templates"] # must be before the ExtensionTask is created

if RUBY_ENGINE == "ruby" and !ENV["PRISM_FFI_BACKEND"]
  require "rake/extensiontask"

  Rake::ExtensionTask.new(:compile) do |ext|
    ext.name = "prism"
    ext.ext_dir = "ext/prism"
    ext.lib_dir = "lib/prism"
    ext.gem_spec = Gem::Specification.load("prism.gemspec")
  end
elsif RUBY_ENGINE == "jruby"
  require "rake/javaextensiontask"

  # This compiles java to make sure any templating changes produces valid code.
  Rake::JavaExtensionTask.new(:compile) do |ext|
    ext.name = "prism"
    ext.ext_dir = "java"
    ext.lib_dir = "tmp"
    ext.source_version = "1.8"
    ext.target_version = "1.8"
    ext.gem_spec = Gem::Specification.load("prism.gemspec")
  end
end

# So `rake clobber` will delete generated files
CLOBBER.concat(Prism::Template::TEMPLATES)
CLOBBER.concat(["build"])
CLOBBER << "lib/prism/prism.#{RbConfig::CONFIG["DLEXT"]}"

Prism::Template::TEMPLATES.each do |filepath|
  desc "Generate #{filepath}"
  file filepath => ["templates/#{filepath}.erb", "templates/template.rb", "config.yml"] do |t|
    Prism::Template.render(t.name)
  end
end

task :vendor_jar do
  ENV["JARS_HOME"]='.'

  require 'jars/installer'
  require 'fileutils'
  require_relative 'lib/prism/version'
  Jars::Installer.vendor_jars!

  # I am doing something weird to bend jars installer to my will.
  FileUtils.rm_rf "home" if FileTest.exist? "home"
  Dir['*.jar'].each do |file|
    FileUtils.rm file unless file == "jruby-prism-#{Prism::VERSION}.jar"
  end

  FileUtils.mv "jruby-prism-#{Prism::VERSION}.jar", 'jruby-prism.jar'
end
