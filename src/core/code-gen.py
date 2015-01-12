
cowsay='cow : @cowsay@'

def template_replace(code_template, code_env):
  for k,v in code_env.items():
    code_template= code_template.replace(k,v)
  return code_template

def read_all_stdin():
  ret=[]
  try:
    for line in iter((raw_input or input),'<EOF>'):
      ret.append(line)
  except EOFError:
    pass

  return '\n'.join(ret)

def main():
  in_str= read_all_stdin()
  cmd_text_arr= in_str.split('@@@',2)
  if len(cmd_text_arr)==3:
    cmd_text= cmd_text_arr[1]
  else:
    cmd_text= cmd_text_arr[0]

  code_template_str, code_env_str= cmd_text.strip().split('\n',1)

  code_template_str= code_template_str.strip()
  code_env_str= code_env_str.strip()

  code_template= eval(code_template_str)
  code_env= eval(code_env_str)

  ret='''/*
cat | python code-gen.py << @@@
'''
  ret+= str(code_template_str)+'\n'
  ret+= str(code_env_str)+'\n'
  ret+= '@@@\n*/\n'

  ret+= template_replace(code_template, code_env)
  print(ret)


if __name__=='__main__':
  main()
  