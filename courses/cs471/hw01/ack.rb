def ack(m, n)
    if m == 0
        return n + 1
    end
    if n == 0
        return ack(m - 1, 1)
    end
    return ack(m - 1, ack(m, n - 1))
end

puts "ack(1, 0) = #{ack(1, 0)}"
puts "ack(0, 3) = #{ack(0, 3)}"

puts "\nack(3, 9) = #{ack(3, 9)}"
