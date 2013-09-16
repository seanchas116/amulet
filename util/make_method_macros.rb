
def output_macro(n)

  arg = n.times.map {|i| "T#{i}, A#{i}, "}.inject(:+)
  array = n.times.map {|i| "(T#{i}, A#{i})"}.inject {|m, x| "#{m}, #{x}"}

  puts "\#define AMULET_METHOD#{n}(NAME, #{arg} ...) \\\n  AMULET_METHOD(NAME, (#{n}, (#{array})), __VA_ARGS__)"

end

(0..22).each do |i|
  output_macro(i)
end

