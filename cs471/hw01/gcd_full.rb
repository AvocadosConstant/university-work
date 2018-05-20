def gcdI(i, j)
    while i != j
        if i > j
            i = i - j
        else
            j = j - i
        end
    end
    return i
end

def gcdR(i, j)
    if i % j == 0
        return j
    end
    return gcdR(j, i % j)
end

if ARGV.length != 2
    puts "Pass two integer arguments" 
    exit
end

i = ARGV[0].to_i
j = ARGV[1].to_i
puts "gcdI(#{i}, #{j}) = #{gcdI(i, j)}"
puts "gcdI(#{i}, #{j}) = #{gcdR(i, j)}"
