def main():
  meow(3)

def meow(times: int):
  if (times == 0):
    return 

  print('meow')
  meow(times - 1)

main()
