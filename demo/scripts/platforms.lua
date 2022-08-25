platforms = {
    { name = "platform1" },
    { name = "platform2" },
    { name = "platform3" }
}

for i, platform in ipairs(platforms) do
    local x, y, z = get_entity_position(platform.name)
    platform.origin = { x = x, y = y, z = z }
end

time = 0

function on_tick(delta_time)
   time = time + delta_time

   for i, platform in ipairs(platforms) do
        set_entity_position(platform.name, platform.origin.x + math.sin(time * i) * 2.0, platform.origin.y, platform.origin.z + math.sin(time * i) * 2.0)
   end
end
