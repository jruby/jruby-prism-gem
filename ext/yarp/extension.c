#include <ruby.h>
#include <yarp.h>

VALUE rb_cYARP;
VALUE rb_cYARPToken;
VALUE rb_cYARPLocation;

static VALUE
location_new(yp_location_t *location) {
  VALUE argv[] = { LONG2FIX(location->start), LONG2FIX(location->end) };
  return rb_class_new_instance(2, argv, rb_cYARPLocation);
}

static VALUE
token_type(yp_token_t *token) {
  if (token->type == YP_TOKEN_INVALID) {
    // We're going to special-case the invalid token here since that doesn't
    // actually exist in Ripper. This is going to give us a little more
    // information when our tests fail.
    // fprintf(stderr, "Invalid token: %.*s\n", (int) (token.end - token.start), token.start);
    return ID2SYM(rb_intern("INVALID"));
  }

  return ID2SYM(rb_intern(yp_token_type_to_str(token->type)));
}

static VALUE
token_new(yp_parser_t *parser, yp_token_t *token) {
  VALUE argv[] = {
    token_type(token),
    rb_str_new(token->start, token->end - token->start),
    location_new(&(yp_location_t) {
      .start = token->start - parser->start,
      .end = token->end - parser->start,
    }),
  };

  return rb_class_new_instance(3, argv, rb_cYARPToken);
}

/******************************************************************************/
/* BEGIN TEMPLATE                                                             */
/******************************************************************************/

static VALUE
node_new(yp_parser_t *parser, yp_node_t *node) {
  switch (node->type) {
    case YP_NODE_ASSIGNMENT: {
      VALUE argv[4];

      // target
      argv[0] = node_new(parser, node->as.assignment.target);

      // operator
      argv[1] = token_new(parser, &node->as.assignment.operator);

      // value
      argv[2] = node_new(parser, node->as.assignment.value);

      // location
      argv[3] = location_new(&node->location);

      return rb_class_new_instance(4, argv, rb_const_get_at(rb_cYARP, rb_intern("Assignment")));
    }
    case YP_NODE_BINARY: {
      VALUE argv[4];

      // left
      argv[0] = node_new(parser, node->as.binary.left);

      // operator
      argv[1] = token_new(parser, &node->as.binary.operator);

      // right
      argv[2] = node_new(parser, node->as.binary.right);

      // location
      argv[3] = location_new(&node->location);

      return rb_class_new_instance(4, argv, rb_const_get_at(rb_cYARP, rb_intern("Binary")));
    }
    case YP_NODE_CALL_NODE: {
      VALUE argv[2];

      // message
      argv[0] = token_new(parser, &node->as.call_node.message);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("CallNode")));
    }
    case YP_NODE_CHARACTER_LITERAL: {
      VALUE argv[2];

      // value
      argv[0] = token_new(parser, &node->as.character_literal.value);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("CharacterLiteral")));
    }
    case YP_NODE_CLASS_VARIABLE_READ: {
      VALUE argv[2];

      // name
      argv[0] = token_new(parser, &node->as.class_variable_read.name);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("ClassVariableRead")));
    }
    case YP_NODE_CLASS_VARIABLE_WRITE: {
      VALUE argv[4];

      // name
      argv[0] = token_new(parser, &node->as.class_variable_write.name);

      // operator
      argv[1] = token_new(parser, &node->as.class_variable_write.operator);

      // value
      argv[2] = node_new(parser, node->as.class_variable_write.value);

      // location
      argv[3] = location_new(&node->location);

      return rb_class_new_instance(4, argv, rb_const_get_at(rb_cYARP, rb_intern("ClassVariableWrite")));
    }
    case YP_NODE_FALSE_NODE: {
      VALUE argv[2];

      // keyword
      argv[0] = token_new(parser, &node->as.false_node.keyword);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("FalseNode")));
    }
    case YP_NODE_FLOAT_LITERAL: {
      VALUE argv[2];

      // value
      argv[0] = token_new(parser, &node->as.float_literal.value);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("FloatLiteral")));
    }
    case YP_NODE_GLOBAL_VARIABLE_READ: {
      VALUE argv[2];

      // name
      argv[0] = token_new(parser, &node->as.global_variable_read.name);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("GlobalVariableRead")));
    }
    case YP_NODE_GLOBAL_VARIABLE_WRITE: {
      VALUE argv[4];

      // name
      argv[0] = token_new(parser, &node->as.global_variable_write.name);

      // operator
      argv[1] = token_new(parser, &node->as.global_variable_write.operator);

      // value
      argv[2] = node_new(parser, node->as.global_variable_write.value);

      // location
      argv[3] = location_new(&node->location);

      return rb_class_new_instance(4, argv, rb_const_get_at(rb_cYARP, rb_intern("GlobalVariableWrite")));
    }
    case YP_NODE_IF_NODE: {
      VALUE argv[4];

      // keyword
      argv[0] = token_new(parser, &node->as.if_node.keyword);

      // predicate
      argv[1] = node_new(parser, node->as.if_node.predicate);

      // statements
      argv[2] = node_new(parser, node->as.if_node.statements);

      // location
      argv[3] = location_new(&node->location);

      return rb_class_new_instance(4, argv, rb_const_get_at(rb_cYARP, rb_intern("IfNode")));
    }
    case YP_NODE_IMAGINARY_LITERAL: {
      VALUE argv[2];

      // value
      argv[0] = token_new(parser, &node->as.imaginary_literal.value);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("ImaginaryLiteral")));
    }
    case YP_NODE_INSTANCE_VARIABLE_READ: {
      VALUE argv[2];

      // name
      argv[0] = token_new(parser, &node->as.instance_variable_read.name);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("InstanceVariableRead")));
    }
    case YP_NODE_INSTANCE_VARIABLE_WRITE: {
      VALUE argv[4];

      // name
      argv[0] = token_new(parser, &node->as.instance_variable_write.name);

      // operator
      argv[1] = token_new(parser, &node->as.instance_variable_write.operator);

      // value
      argv[2] = node_new(parser, node->as.instance_variable_write.value);

      // location
      argv[3] = location_new(&node->location);

      return rb_class_new_instance(4, argv, rb_const_get_at(rb_cYARP, rb_intern("InstanceVariableWrite")));
    }
    case YP_NODE_INTEGER_LITERAL: {
      VALUE argv[2];

      // value
      argv[0] = token_new(parser, &node->as.integer_literal.value);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("IntegerLiteral")));
    }
    case YP_NODE_LOCAL_VARIABLE_READ: {
      VALUE argv[2];

      // name
      argv[0] = token_new(parser, &node->as.local_variable_read.name);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("LocalVariableRead")));
    }
    case YP_NODE_LOCAL_VARIABLE_WRITE: {
      VALUE argv[4];

      // name
      argv[0] = token_new(parser, &node->as.local_variable_write.name);

      // operator
      argv[1] = token_new(parser, &node->as.local_variable_write.operator);

      // value
      argv[2] = node_new(parser, node->as.local_variable_write.value);

      // location
      argv[3] = location_new(&node->location);

      return rb_class_new_instance(4, argv, rb_const_get_at(rb_cYARP, rb_intern("LocalVariableWrite")));
    }
    case YP_NODE_NIL_NODE: {
      VALUE argv[2];

      // keyword
      argv[0] = token_new(parser, &node->as.nil_node.keyword);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("NilNode")));
    }
    case YP_NODE_OPERATOR_ASSIGNMENT: {
      VALUE argv[4];

      // target
      argv[0] = node_new(parser, node->as.operator_assignment.target);

      // operator
      argv[1] = token_new(parser, &node->as.operator_assignment.operator);

      // value
      argv[2] = node_new(parser, node->as.operator_assignment.value);

      // location
      argv[3] = location_new(&node->location);

      return rb_class_new_instance(4, argv, rb_const_get_at(rb_cYARP, rb_intern("OperatorAssignment")));
    }
    case YP_NODE_PROGRAM: {
      VALUE argv[3];

      // scope
      argv[0] = node_new(parser, node->as.program.scope);

      // statements
      argv[1] = node_new(parser, node->as.program.statements);

      // location
      argv[2] = location_new(&node->location);

      return rb_class_new_instance(3, argv, rb_const_get_at(rb_cYARP, rb_intern("Program")));
    }
    case YP_NODE_RATIONAL_LITERAL: {
      VALUE argv[2];

      // value
      argv[0] = token_new(parser, &node->as.rational_literal.value);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("RationalLiteral")));
    }
    case YP_NODE_REDO: {
      VALUE argv[2];

      // value
      argv[0] = token_new(parser, &node->as.redo.value);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("Redo")));
    }
    case YP_NODE_RETRY: {
      VALUE argv[2];

      // value
      argv[0] = token_new(parser, &node->as.retry.value);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("Retry")));
    }
    case YP_NODE_SCOPE: {
      VALUE argv[2];

      // locals
      argv[0] = rb_ary_new();
      for (size_t index = 0; index < node->as.scope.locals->size; index++) {
        rb_ary_push(argv[0], token_new(parser, &node->as.scope.locals->tokens[index]));
      }

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("Scope")));
    }
    case YP_NODE_SELF_NODE: {
      VALUE argv[2];

      // keyword
      argv[0] = token_new(parser, &node->as.self_node.keyword);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("SelfNode")));
    }
    case YP_NODE_STATEMENTS: {
      VALUE argv[2];

      // body
      argv[0] = rb_ary_new();
      for (size_t index = 0; index < node->as.statements.body->size; index++) {
        rb_ary_push(argv[0], node_new(parser, node->as.statements.body->nodes[index]));
      }

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("Statements")));
    }
    case YP_NODE_TERNARY: {
      VALUE argv[6];

      // predicate
      argv[0] = node_new(parser, node->as.ternary.predicate);

      // question_mark
      argv[1] = token_new(parser, &node->as.ternary.question_mark);

      // true_expression
      argv[2] = node_new(parser, node->as.ternary.true_expression);

      // colon
      argv[3] = token_new(parser, &node->as.ternary.colon);

      // false_expression
      argv[4] = node_new(parser, node->as.ternary.false_expression);

      // location
      argv[5] = location_new(&node->location);

      return rb_class_new_instance(6, argv, rb_const_get_at(rb_cYARP, rb_intern("Ternary")));
    }
    case YP_NODE_TRUE_NODE: {
      VALUE argv[2];

      // keyword
      argv[0] = token_new(parser, &node->as.true_node.keyword);

      // location
      argv[1] = location_new(&node->location);

      return rb_class_new_instance(2, argv, rb_const_get_at(rb_cYARP, rb_intern("TrueNode")));
    }
    case YP_NODE_UNLESS_NODE: {
      VALUE argv[4];

      // keyword
      argv[0] = token_new(parser, &node->as.unless_node.keyword);

      // predicate
      argv[1] = node_new(parser, node->as.unless_node.predicate);

      // statement
      argv[2] = node_new(parser, node->as.unless_node.statement);

      // location
      argv[3] = location_new(&node->location);

      return rb_class_new_instance(4, argv, rb_const_get_at(rb_cYARP, rb_intern("UnlessNode")));
    }
    case YP_NODE_UNTIL_NODE: {
      VALUE argv[4];

      // keyword
      argv[0] = token_new(parser, &node->as.until_node.keyword);

      // predicate
      argv[1] = node_new(parser, node->as.until_node.predicate);

      // statement
      argv[2] = node_new(parser, node->as.until_node.statement);

      // location
      argv[3] = location_new(&node->location);

      return rb_class_new_instance(4, argv, rb_const_get_at(rb_cYARP, rb_intern("UntilNode")));
    }
    case YP_NODE_WHILE_NODE: {
      VALUE argv[4];

      // keyword
      argv[0] = token_new(parser, &node->as.while_node.keyword);

      // predicate
      argv[1] = node_new(parser, node->as.while_node.predicate);

      // statement
      argv[2] = node_new(parser, node->as.while_node.statement);

      // location
      argv[3] = location_new(&node->location);

      return rb_class_new_instance(4, argv, rb_const_get_at(rb_cYARP, rb_intern("WhileNode")));
    }
    default:
      rb_raise(rb_eRuntimeError, "unknown node type: %d", node->type);
  }
}

/******************************************************************************/
/* END TEMPLATE                                                               */
/******************************************************************************/

// Represents a source of Ruby code. It can either be coming from a file or a
// string. If it's a file, it's going to mmap the contents of the file. If it's
// a string it's going to just point to the contents of the string.
typedef struct {
  enum { SOURCE_FILE, SOURCE_STRING } type;
  const char *source;
  off_t size;
} source_t;

// Read the file indicated by the filepath parameter into source and load its
// contents and size into the given source_t.
int
source_file_load(source_t *source, VALUE filepath) {
  // Open the file for reading
  int fd = open(StringValueCStr(filepath), O_RDONLY);
  if (fd == -1) {
    perror("open");
    return 1;
  }

  // Stat the file to get the file size
  struct stat sb;
  if (fstat(fd, &sb) == -1) {
    close(fd);
    perror("fstat");
    return 1;
  }

  // mmap the file descriptor to virtually get the contents
  source->size = sb.st_size;
  source->source = mmap(NULL, source->size, PROT_READ, MAP_PRIVATE, fd, 0);

  close(fd);
  if (source == MAP_FAILED) {
    perror("mmap");
    return 1;
  }

  return 0;
}

// Load the contents and size of the given string into the given source_t.
void
source_string_load(source_t *source, VALUE string) {
  *source = (source_t) {
    .type = SOURCE_STRING,
    .source = StringValueCStr(string),
    .size = RSTRING_LEN(string),
  };
}

// Free any resources associated with the given source_t.
void
source_file_unload(source_t *source) {
  munmap((void *) source->source, source->size);
}

// Dump the AST corresponding to the given source to a string.
static VALUE
dump_source(source_t *source) {
  yp_parser_t parser;
  yp_parser_init(&parser, source->source, source->size);

  yp_node_t *node = yp_parse(&parser);
  yp_buffer_t *buffer = yp_buffer_alloc();
  yp_serialize(&parser, node, buffer);

  VALUE dumped = rb_str_new(buffer->value, buffer->length);
  yp_node_dealloc(&parser, node);
  yp_buffer_free(buffer);

  return dumped;
}

// Dump the AST corresponding to the given string to a string.
static VALUE
dump(VALUE self, VALUE string) {
  source_t source;
  source_string_load(&source, string);
  return dump_source(&source);
}

// Dump the AST corresponding to the given file to a string.
static VALUE
dump_file(VALUE self, VALUE filepath) {
  source_t source;
  if (source_file_load(&source, filepath) != 0) return Qnil;

  VALUE value = dump_source(&source);
  source_file_unload(&source);
  return value;
}

// Return an array of tokens corresponding to the given source.
static VALUE
lex_source(source_t *source) {
  yp_parser_t parser;
  yp_parser_init(&parser, source->source, source->size);

  VALUE ary = rb_ary_new();
  for (yp_lex_token(&parser); parser.current.type != YP_TOKEN_EOF; yp_lex_token(&parser)) {
    rb_ary_push(ary, token_new(&parser, &parser.current));
  }

  return ary;
}

// Return an array of tokens corresponding to the given string.
static VALUE
lex(VALUE self, VALUE string) {
  source_t source;
  source_string_load(&source, string);
  return lex_source(&source);
}

// Return an array of tokens corresponding to the given file.
static VALUE
lex_file(VALUE self, VALUE filepath) {
  source_t source;
  if (source_file_load(&source, filepath) != 0) return Qnil;

  VALUE value = lex_source(&source);
  source_file_unload(&source);
  return value;
}

static VALUE
parse_source(source_t *source) {
  yp_parser_t parser;
  yp_parser_init(&parser, source->source, source->size);

  yp_node_t *node = yp_parse(&parser);
  VALUE value = node_new(&parser, node);

  yp_node_dealloc(&parser, node);
  return value;
}

static VALUE
parse(VALUE self, VALUE string) {
  source_t source;
  source_string_load(&source, string);
  return parse_source(&source);
}

static VALUE
parse_file(VALUE self, VALUE rb_filepath) {
  source_t source;
  if (source_file_load(&source, rb_filepath) != 0) {
    return Qnil;
  }

  VALUE value = parse_source(&source);
  source_file_unload(&source);
  return value;
}

void
Init_yarp(void) {
  rb_cYARP = rb_define_module("YARP");
  rb_cYARPToken = rb_define_class_under(rb_cYARP, "Token", rb_cObject);
  rb_cYARPLocation = rb_define_class_under(rb_cYARP, "Location", rb_cObject);

  rb_define_const(rb_cYARP, "VERSION", rb_sprintf("%d.%d.%d", YP_VERSION_MAJOR, YP_VERSION_MINOR, YP_VERSION_PATCH));

  rb_define_singleton_method(rb_cYARP, "dump", dump, 1);
  rb_define_singleton_method(rb_cYARP, "dump_file", dump_file, 1);

  rb_define_singleton_method(rb_cYARP, "lex", lex, 1);
  rb_define_singleton_method(rb_cYARP, "lex_file", lex_file, 1);

  rb_define_singleton_method(rb_cYARP, "parse", parse, 1);
  rb_define_singleton_method(rb_cYARP, "parse_file", parse_file, 1);
}
