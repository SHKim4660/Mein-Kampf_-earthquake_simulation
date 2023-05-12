bin := MCEQ

c++ := c++
c++flags := -Wall -Wpedantic -g -ggdb3 -std=c++17
ldflags := -lglbinding -lglbinding-aux -lglfw

objs := main.o utils.o shader.o

.Phony : run clean
all: $(bin) $(test)

deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

$(bin): $(objs)
	$(c++) $(c++flags) $(ldflags) -o $@ $^

%.o: %.cpp
	$(c++) $(c++flags) -c $< $(DEPFLAGS)

clean:
	rm -f $(objs) $(bin)

run: $(bin)
	./$(bin)
