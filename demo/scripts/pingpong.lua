-- Horizontale Richtung
local horizontal_direction = "right"
-- Vertikale Richtung
local vertical_direction = "up"

-- Geschwindigkeit
local speed = 6.0

-- Funktion wird jeden Frame aufgerufen
function on_tick(delta_time)
    local x, y, z = get_object_position("cube")
    if x > 9.0 then -- Rechte Seite, nach links bewegen
        horizontal_direction = "left"
    elseif x < -9.0 then -- Linke Seite, nach rechts bewegen
        horizontal_direction = "right"
    end
    if z > 9.0 then -- Obere Seite, nach unten bewegen
        vertical_direction = "down"
    elseif z < -9.0 then -- Obere Seite, nach oben bewegen
        vertical_direction = "up"
    end

    if horizontal_direction == "right" then -- Rechts
        x = x + speed * delta_time
    elseif horizontal_direction == "left" then -- Links
        x = x - speed * delta_time
    end
    if vertical_direction == "up" then -- Oben
        z = z + speed * delta_time
    elseif vertical_direction == "down" then -- Unten
        z = z - speed * delta_time
    end

    -- Neue Position setzen
    set_object_position("cube", x, y, z)
end