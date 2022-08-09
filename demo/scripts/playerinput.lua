-- Geschwindigkeit
local speed = 6.0

-- Funktion wird jeden Frame aufgerufen
function on_tick(delta_time)
    local x, y, z = get_object_position("cube")

    if is_key_pressed(0x57) then -- W
        z = z + speed * delta_time
    elseif is_key_pressed(0x53) then -- S
        z = z - speed * delta_time
    end
    if is_key_pressed(0x44) then -- D
        x = x - speed * delta_time
    elseif is_key_pressed(0x41) then -- A
        x = x + speed * delta_time
    end

    -- Sicherstellen, dass die Figur sich nicht außerhalb der Fläche bewegt
    if x > 9.0 then
        x = 9.0
    elseif x < -9.0 then
        x = -9.0
    end
    if z > 9.0 then
        z = 9.0
    elseif z < -9.0 then
        z = -9.0
    end

    -- Neue Position setzen
    set_object_position("cube", x, y, z)
end