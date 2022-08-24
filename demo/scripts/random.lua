-- Richtung
local direction = 0

-- Geschwindigkeit
local speed = 8.0

-- Gesammelte Framezeit
local accumulated_deltatime = 0.0

-- Funktion wird jeden Frame aufgerufen
function on_tick(delta_time)
    local x, y, z = get_entity_position("cube")

    -- Richtung ändern alle 0.5 Sekunden
    accumulated_deltatime = accumulated_deltatime + delta_time
    if accumulated_deltatime > 0.5 then
        accumulated_deltatime = 0.0
        direction = math.random(5)
    end

    -- Bewegen nach:
    if direction == 1 then -- Oben
        z = z + speed * delta_time
    elseif direction == 2 then -- Rechts
        x = x + speed * delta_time
    elseif direction == 3 then -- Unten
        z = z - speed * delta_time 
    elseif direction == 4 then -- Links
        x = x - speed * delta_time
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
    set_entity_position("cube", x, y, z)
end