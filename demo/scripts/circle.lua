-- Momentane Richtung
local direction = "right"

-- Geschwindigkeit
local speed = 6.0

-- Funktion wird jeden Frame aufgerufen
function on_tick(delta_time)
    local x, y, z = get_object_position("cube")
    if x > 9.0 then -- Rechte Seite, nach unten bewegen
        x = 9.0
        direction = "down"
    elseif x < -9.0 then -- Linke Seite, nach oben bewegen
        x = -9.0
        direction = "up"
    end
    if z > 9.0 then -- Obere Seite, nach rechts bewegen
        z = 9.0
        direction = "right"
    elseif z < -9.0 then -- Untere Seite, nach links bewegen
        z = -9.0
        direction = "left"
    end

    if direction == "up" then -- Oben
        z = z + speed * delta_time
    elseif direction == "right" then -- Rechts
        x = x + speed * delta_time
    elseif direction == "down" then -- Unten
        z = z - speed * delta_time 
    elseif direction == "left" then -- Links
        x = x - speed * delta_time
    end

    -- Neue Position
    set_object_position("cube", x, y, z)
end
